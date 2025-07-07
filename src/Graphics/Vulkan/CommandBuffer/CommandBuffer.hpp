#pragma once
#include "Graphics/ICommandBuffer.hpp"
#include "Graphics/Synchronization.hpp"
#include "Graphics/Vulkan/Queue.hpp"
#include "Graphics/Vulkan/Synchronization/Fence.hpp"
#include <deque>

#include <pch.h>
#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan_enums.hpp>
#include <vulkan/vulkan_handles.hpp>
#include <vulkan/vulkan_structs.hpp>
namespace VK
{
struct Command : ICommand
{
};
using CmdCapatiblities = QueueCapabilities;
class CommandBuffer : public ICommandBuffer
{
    vk::CommandPool commandPool;
    struct Partition // commands split into separate buffers since semaphore/fence operations only happen on begin/end
    {
        Partition();
        ~Partition();
        vk::CommandBuffer cmdBufferHandle;
        std::vector<std::unique_ptr<Command>> commands;
        GPURef<Graphics::Semaphore> SignalSemaphore,
            WaitSemaphore; // waits only matter at begin, singal only at end
        GPURef<VK::Fence> CompletionFence;
    };
    std::deque<Partition> Partitions;
    bool PartitionsDirty = true;
    void MakePartition();
    const Flags<CmdCapatiblities> Capabilities;

  public:
    CommandBuffer(const Flags<CmdCapatiblities> &Capabilities = {CmdCapatiblities::eGeneric});
    void SignalSemaphore(GPURef<Graphics::Semaphore> semaphore) override
    {
        Partitions.back().SignalSemaphore = semaphore;
        MakePartition();
        PartitionsDirty = true;
    }
    [[nodiscard]] GPURef<Graphics::Semaphore> SignalSemaphore() override
    {
        GPURef<Graphics::Semaphore> Semaphore = GPURef<Graphics::Semaphore>::MakeRef();
        SignalSemaphore(Semaphore);
        return Semaphore;
    }
    void WaitSemaphore(GPURef<Graphics::Semaphore> semaphore) override
    {
        MakePartition();
        Partitions.back().WaitSemaphore = semaphore;
        PartitionsDirty = true;
    }
    [[nodiscard]] GPURef<Graphics::Semaphore> WaitSemaphore() override
    {
        GPURef<Graphics::Semaphore> Semaphore = GPURef<Graphics::Semaphore>::MakeRef();
        WaitSemaphore(Semaphore);
        return Semaphore;
    }
    void SignalFence(GPURef<VK::Fence> fence)
    {
        Partitions.back().CompletionFence = fence;
        MakePartition();
        PartitionsDirty = true;
    }
    [[nodiscard]] GPURef<VK::Fence> SignalFence()
    {
        GPURef<VK::Fence> Fence = GPURef<VK::Fence>::MakeRef();
        SignalFence(Fence);
        return Fence;
    }
    void BeginRender(GPURef<Graphics::RenderTarget> rt) override {}
    void EndRender(GPURef<Graphics::RenderTarget> rt)override{}
    [[nodiscard]] vk::CommandBuffer GetReadyHandle()
    {
        if (Partitions.empty())
            Partitions.emplace_back();
        return Partitions.back().cmdBufferHandle;
    }
    void Submit(GPURef<VK::Queue> queue)
    {
        if (PartitionsDirty)
        {
            if (Partitions.back().commands.empty())
                Partitions.pop_back();
            PartitionsDirty = false;
            for (auto &Partition : Partitions)
            {
                Partition.cmdBufferHandle.reset();

                const auto BeginResult = Partition.cmdBufferHandle.begin(vk::CommandBufferBeginInfo{});
                LASSERT(BeginResult == vk::Result::eSuccess, "Darn");
                for (const auto &Command : Partition.commands)
                {
                    Command->Execute(&Partition.cmdBufferHandle);
                }
                const auto EndResult = Partition.cmdBufferHandle.end();
                LASSERT(EndResult == vk::Result::eSuccess, "Darn");
            }
        }

        for (const auto &Partition : Partitions)
        {
            vk::SubmitInfo SubmitInfo;
            SubmitInfo.setCommandBuffers({Partition.cmdBufferHandle});
            vk::Semaphore WaitSemaphoreHandle, SignalSemaphoreHandle;
            const vk::PipelineStageFlags WaitStageMask = vk::PipelineStageFlagBits::eAllCommands;
            ;
            SubmitInfo.pWaitDstStageMask = &WaitStageMask;
            if (Partition.WaitSemaphore)
            {
                WaitSemaphoreHandle = Partition.WaitSemaphore->VK().GetHandle();
                SubmitInfo.setWaitSemaphores({WaitSemaphoreHandle});
            }
            if (Partition.SignalSemaphore)
            {
                SignalSemaphoreHandle = Partition.SignalSemaphore->VK().GetHandle();
                SubmitInfo.setSignalSemaphores({SignalSemaphoreHandle});
            }

            const auto SubmitResult = queue->GetHandle().submit(
                SubmitInfo, Partition.CompletionFence ? Partition.CompletionFence->GetHandle() : VK_NULL_HANDLE);
        }
    }
    void Clear() override
    {
        Partitions.clear();
        PartitionsDirty = true;
    }
    template <typename T, typename... ARGS> void Push(ARGS... args)
    {
        // Check that the API commands given correspond to the API of this command buffer
        const bool ValidCommand = (std::is_base_of_v<VK::Command, T>);
        LASSERT(ValidCommand, "Invalid Command");
        if (Partitions.empty())
            MakePartition();

        Partitions.back().commands.push_back(std::make_unique<T>(std::forward<ARGS>(args)...));
        PartitionsDirty = true;
    };
    [[nodiscard]] auto GetCapabilities() const
    {
        return Capabilities;
    }
};
} // namespace VK
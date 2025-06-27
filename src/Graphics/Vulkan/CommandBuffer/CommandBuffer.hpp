#pragma once
#include "Graphics/ICommandBuffer.hpp"
#include "Graphics/Synchronization.hpp"
#include <deque>

#include <pch.h>
#include <vulkan/vulkan_enums.hpp>
#include <vulkan/vulkan_handles.hpp>
#include <vulkan/vulkan_structs.hpp>

namespace VK
{
struct Command : ICommand
{
};
class CommandBuffer : public ICommandBuffer
{
    vk::CommandPool commandPool;
    struct Partition // commands split into separate buffers since semaphore/fence operations only happen on begin/end
    {
        vk::CommandBuffer cmdBufferHandle;
        std::vector<std::unique_ptr<Command>> commands;
        GPURef<Graphics::Semaphore> SignalSemaphore,
            WaitSemaphore; // waits only matter at begin, singal only at end

    };
    std::deque<Partition> Partitions;
    bool PartitionsDirty = true;
    void MakePartition();

  public:
    CommandBuffer();
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
    void SignalFence(GPURef<Graphics::Semaphore> semaphore) override
    {
        PartitionsDirty = true;
    }
    void WaitFence(GPURef<Graphics::Semaphore> semaphore) override
    {
        PartitionsDirty = true;
    }
    void Submit(vk::Queue queue)
    {
        if (PartitionsDirty)
        {
            if (Partitions.back().commands.empty())
                Partitions.pop_back();
            PartitionsDirty = false;
            for (auto &Partition : Partitions)
            {
                Partition.cmdBufferHandle.reset();

                Partition.cmdBufferHandle.begin(vk::CommandBufferBeginInfo{});
                for (const auto &Command : Partition.commands)
                {
                    Command->Execute(&Partition.cmdBufferHandle);
                }
                Partition.cmdBufferHandle.end();
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
            const auto SubmitResult = queue.submit(SubmitInfo);
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
};
} // namespace VK
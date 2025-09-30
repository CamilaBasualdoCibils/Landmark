#pragma once
#include "Graphics/ICommandManager.hpp"
#include "Graphics/Synchronization.hpp"
#include "Graphics/Vulkan/Commands/VKCommandBuffer.hpp"
#include "Graphics/Vulkan/Commands/VKCommandPool.hpp"
#include "Graphics/Vulkan/VKQueue.hpp"
#include "Graphics/Vulkan/Synchronization/VKFence.hpp"
#include <pch.h>
namespace VK
{
struct Command : Graphics::ICommand<GPURef<VK::CommandBuffer>>
{
};
using CmdCapatiblities = QueueCapabilities;
class CommandManager : public Graphics::ICommandManager
{
    GPURef<VK::CommandPool> CmdPool;
    struct Partition // commands split into separate buffers since semaphore/fence operations only happen on begin/end
    {
        Partition();
        ~Partition();
        bool ManualParition = false;
        GPURef<VK::CommandBuffer> CmdBuffer;
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
    CommandManager(const Flags<CmdCapatiblities> &Capabilities = {CmdCapatiblities::eGeneric});
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
    void BeginRendering(GPURef<VK::RenderTarget> rt);
    void EndRendering();
    void Submit(GPURef<VK::Queue> queue);
    void Clear() override;
    void InsertLabel(const std::string &Label, vec4 Color = {1, 1, 1, 1}) override;
    void PushGroupLabel(const std::string &Group, vec4 Color = {1, 1, 1, 1}) override;
    void PopGroupLabel() override;

    /// @brief Manual Command buffer. the Wait/Signal Semaphores/fences functions will not work as expected and will
    /// only engage before or after the returned command buffer
    /// @return
    [[nodiscard]] GPURef<VK::CommandBuffer> GetManualCommandBuffer();
    template <typename T, typename... ARGS> void Push(ARGS... args)
    {
        // Check that the API commands given correspond to the API of this command buffer
        const bool ValidCommand = (std::is_base_of_v<VK::Command, T>);
        LASSERT(ValidCommand, "Invalid Command");
        if (Partitions.empty() || Partitions.back().ManualParition)
            MakePartition();

        Partitions.back().commands.push_back(std::make_unique<T>(std::forward<ARGS>(args)...));
        PartitionsDirty = true;
    };
    template <typename T> void Push(const T &cmd)
    {
        // Check that the API commands given correspond to the API of this command buffer
        const bool ValidCommand = (std::is_base_of_v<VK::Command, T>);
        LASSERT(ValidCommand, "Invalid Command");
        if (Partitions.empty() || Partitions.back().ManualParition)
            MakePartition();

        Partitions.back().commands.push_back(std::make_unique<T>(cmd));
        PartitionsDirty = true;
    };
    [[nodiscard]] auto GetCapabilities() const
    {
        return Capabilities;
    }
    size_t Count() const override {
        size_t count = 0;
        for (const auto& Partition : Partitions)
        {
            count += Partition.commands.size();
            count += Partition.SignalSemaphore ? 1 : 0;
            count += Partition.WaitSemaphore ? 1 : 0;
            count += Partition.CompletionFence ? 1 : 0;
        }
        return count;
    }
};
} // namespace VK
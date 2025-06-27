#pragma once
#include "Graphics/ICommandBuffer.hpp"
#include "Graphics/Synchronization.hpp"
#include <deque>

#include <pch.h>

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
        std::vector<std::vector<std::unique_ptr<Command>>> commands;
        std::vector<GPURef<Graphics::Semaphore>> SignalSemaphores, WaitSemaphores;
    };
    std::deque<Partition> Partitions;

    void MakePartition();

  public:
    CommandBuffer();
    void SignalSemaphore(GPURef<Graphics::Semaphore> semaphore) override {}
    void WaitSemaphore(GPURef<Graphics::Semaphore> semaphore) override {}
    void SignalFence(GPURef<Graphics::Semaphore> semaphore) override
    {
    }
    void WaitFence(GPURef<Graphics::Semaphore> semaphore) override
    {
    }
    void Submit()
    {
    }
    void Clear() override {}
    template <typename T, typename... ARGS> void Push(ARGS... args)
    {
        // Check that the API commands given correspond to the API of this command buffer
        const bool ValidCommand = !(std::is_base_of_v<VK::Command, T>);
        LASSERT(ValidCommand, "Invalid Command");
        if (Partitions.empty())
            MakePartition();

        Partitions.back().commands.push_back(std::make_unique<T>(std::forward<ARGS>(args)...));
    };
};
} // namespace VK
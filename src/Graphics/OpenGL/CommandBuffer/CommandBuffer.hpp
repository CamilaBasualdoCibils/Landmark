#pragma once
#include <Graphics/ICommandBuffer.hpp>
#include <pch.h>

namespace GL
{
struct Command : ICommand
{

};
class CommandBuffer : public ICommandBuffer
{
    std::deque<std::unique_ptr<ICommand>> Commands;

  public:
    CommandBuffer();
    void SignalSemaphore(GPURef<Graphics::Semaphore> semaphore) override;
    [[nodiscard]] GPURef<Graphics::Semaphore> SignalSemaphore()override;
    void WaitSemaphore(GPURef<Graphics::Semaphore> semaphore) override;
    [[nodiscard]] GPURef<Graphics::Semaphore> WaitSemaphore() override;
    void SignalFence(GPURef<Graphics::Semaphore> semaphore) override
    {
    }
    void WaitFence(GPURef<Graphics::Semaphore> semaphore) override
    {
    }
    void Submit();

    const decltype(Commands) &GetCommands() const
    {
        return Commands;
    }
    template <typename T, typename... ARGS> void Push(ARGS... args)
    {
        // Check that the API commands given correspond to the API of this command buffer
        const bool ValidCommand = (std::is_base_of_v<GL::Command, T>);
        LASSERT(ValidCommand, "Invalid Command");

        Commands.push_back(std::make_unique<T>(std::forward<ARGS>(args)...));
    };
    void Clear() override;
};
} // namespace GL
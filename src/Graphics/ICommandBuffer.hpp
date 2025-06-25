#pragma once
#include <Graphics/GPURef.hpp>
#include <pch.h>
#include <utility>
class Semaphore;
class Fence;
struct Command
{
    virtual void Execute(void *context) = 0;
    virtual std::string GetName() = 0;
};
class ICommandBuffer
{

    std::deque<std::unique_ptr<Command>> Commands;

  public:
    const decltype(Commands) &GetCommands() const {return Commands;}

    virtual void SignalSemaphore(GPURef<Semaphore> semaphore) = 0;
    virtual void WaitSemaphore(GPURef<Semaphore> semaphore) = 0;
    virtual void SignalFence(GPURef<Semaphore> semaphore) = 0;
    virtual void WaitFence(GPURef<Semaphore> semaphore) = 0;
    template <typename T, typename... ARGS> void Push(ARGS... args)
    {
        Commands.push_back(std::make_unique<T>(std::forward(args...)));
    };
};
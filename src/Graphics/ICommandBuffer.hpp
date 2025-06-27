#pragma once
#include <Graphics/GPURef.hpp>
#include <pch.h>
#include <type_traits>
#include <utility>
#include <Graphics/Synchronization.hpp>
class Fence;
struct ICommand
{
    virtual void Execute(void *context) = 0;
    virtual std::string GetName() = 0;
};
namespace GL
{
class CommandBuffer;
class Command;
} // namespace GL

class ICommandBuffer
{
  public:
    virtual void SignalSemaphore(GPURef<Graphics::Semaphore> semaphore) = 0;
    virtual void WaitSemaphore(GPURef<Graphics::Semaphore> semaphore) = 0;
    virtual void SignalFence(GPURef<Graphics::Semaphore> semaphore) = 0;
    virtual void WaitFence(GPURef<Graphics::Semaphore> semaphore) = 0;
    virtual void Clear() = 0;
};
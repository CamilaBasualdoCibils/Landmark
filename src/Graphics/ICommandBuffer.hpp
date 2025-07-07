#pragma once
#include "Graphics/Rendering/RenderTarget.hpp"
#include <Graphics/GPURef.hpp>
#include <Graphics/Synchronization.hpp>
#include <pch.h>
#include <type_traits>
#include <utility>
class Fence;
struct ICommand
{
    virtual void Execute(void *context) = 0;
    virtual std::string GetName() = 0;
    virtual ~ICommand() {}
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
    [[nodiscard]] virtual GPURef<Graphics::Semaphore> SignalSemaphore() = 0 ;
    virtual void WaitSemaphore(GPURef<Graphics::Semaphore> semaphore) = 0;
    [[nodiscard]] virtual GPURef<Graphics::Semaphore> WaitSemaphore() = 0 ;
    virtual void BeginRender(GPURef<Graphics::RenderTarget> rt) = 0;
    virtual void EndRender(GPURef<Graphics::RenderTarget> rt) = 0;

    virtual void Clear() = 0;
};
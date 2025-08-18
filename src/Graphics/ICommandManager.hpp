#pragma once
#include "Graphics/Rendering/RenderTarget.hpp"
#include <Graphics/GPURef.hpp>
#include <Graphics/Synchronization.hpp>
#include <pch.h>
#include <type_traits>
#include <utility>
namespace Graphics
{
class Fence;
template <typename ExecuteArg = void> struct ICommand
{

    virtual void Execute(ExecuteArg context) = 0;
    virtual std::string GetName() = 0;
    virtual ~ICommand()
    {
    }
};

class ICommandManager
{
  public:
    virtual void SignalSemaphore(GPURef<Graphics::Semaphore> semaphore) = 0;
    [[nodiscard]] virtual GPURef<Graphics::Semaphore> SignalSemaphore() = 0;
    virtual void WaitSemaphore(GPURef<Graphics::Semaphore> semaphore) = 0;
    [[nodiscard]] virtual GPURef<Graphics::Semaphore> WaitSemaphore() = 0;
    virtual void Clear() = 0;
    virtual void InsertLabel(const std::string& Label, vec4 Color = {1,1,1,1}) = 0;
    virtual void PushGroupLabel(const std::string& Group, vec4 Color = {1,1,1,1}) = 0;
    virtual void PopGroupLabel() = 0;
    virtual size_t Count() const = 0;

};
} // namespace Graphics

#pragma once
#include <Graphics/ICommandBuffer.hpp>
#include <pch.h>

namespace GL
{

class CommandBuffer : public ICommandBuffer
{
  public:
    CommandBuffer();
    void SignalSemaphore(GPURef<Semaphore> semaphore) override;
    void WaitSemaphore(GPURef<Semaphore> semaphore) override;
    void SignalFence(GPURef<Semaphore> semaphore) override;
    void WaitFence(GPURef<Semaphore> semaphore) override;
    void Submit();
};
} // namespace GL
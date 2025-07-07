#pragma once
#include <pch.h>

namespace VK
{
class Fence
{
    vk::Fence Handle;

  public:
    Fence();
    operator vk::Fence() const {return Handle;}
    operator vk::Fence() {return Handle;}
    
    [[nodiscard]] vk::Fence GetHandle() const {return Handle;}
    [[nodiscard]] vk::Fence GetHandle() {return Handle;}
    void Wait();
    void Reset();
    void WaitAndReset();

    bool IsSignaled() const;
};
} // namespace VK
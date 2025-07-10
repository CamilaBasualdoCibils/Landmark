#pragma once
#include <pch.h>
#include <Graphics/Vulkan/Queue.hpp>
namespace VK
{
    class CommandPool
    {
        vk::CommandPool Handle;

        public:
        CommandPool(GPURef<VK::Queue> Queue);
        ~CommandPool();
        operator vk::CommandPool() const {return Handle;}
    };
} // namespace VK

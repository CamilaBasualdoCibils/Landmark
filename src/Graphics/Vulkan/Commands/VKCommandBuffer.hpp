#pragma once
#include <pch.h>
#include "VKCommandPool.hpp"
namespace VK
{
    class CommandBuffer
    {
        vk::CommandBuffer Handle;
        GPURef<VK::CommandPool> Pool;
        bool Recording = false;
        public:
        CommandBuffer(GPURef<VK::CommandPool> Pool);
        ~CommandBuffer();

        operator vk::CommandBuffer() const {return Handle;}
        vk::CommandBuffer GetHandle() const {return Handle;}
        void Begin();
        void End();
        void Reset();
        [[nodiscard]] bool IsRecording() const {return Recording;}
    };
} // namespace VK

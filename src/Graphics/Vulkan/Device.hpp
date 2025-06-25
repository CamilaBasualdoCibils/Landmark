#pragma once
#include <pch.h>
#include "PhysicalDevice.hpp"
namespace VK
{
    struct DeviceProperties
    {

    };
    class Device : public PhysicalDevice
    {
        friend PhysicalDevice;
        vk::Device DeviceHandle;
        public:
        Device(std::shared_ptr<PhysicalDevice> phyDev,const DeviceProperties& properties);

    };
}
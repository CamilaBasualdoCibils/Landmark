#pragma once
#include <pch.h>
namespace VK
{
    class PhysicalDevice
    {
        vk::PhysicalDevice Handle;
        vk::PhysicalDeviceProperties PhysicalDeviceProperties;
        vk::PhysicalDeviceProperties2 PhysicalDeviceProperties2;
        vk::PhysicalDeviceIDProperties PhysicalDeviceIDProperties;
        public:
        PhysicalDevice(const vk::PhysicalDevice& phDev);

        [[nodiscard]] auto GetDeviceUUID() const {return PhysicalDeviceIDProperties.deviceUUID;}
        [[nodiscard]] auto GetDriverUUID() const {return PhysicalDeviceIDProperties.driverUUID;}
        [[nodiscard]] std::string GetName() const {return PhysicalDeviceProperties.deviceName.data();}
        [[nodiscard]] vk::PhysicalDevice GetVkPhysicalDeviceHandle() const {return Handle;}

        const vk::PhysicalDevice* operator->() const {return &Handle;}
        vk::PhysicalDevice* operator->() {return &Handle;}
    };
}

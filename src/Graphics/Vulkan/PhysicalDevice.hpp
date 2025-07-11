#pragma once
#include <pch.h>
#include "Graphics/Vulkan/Memory/Memory.hpp"
#include "Types/Flags.hpp"
namespace VK
{
class PhysicalDevice
{
    vk::PhysicalDevice Handle;
    vk::PhysicalDeviceProperties PhysicalDeviceProperties;
    vk::PhysicalDeviceProperties2 PhysicalDeviceProperties2;
    vk::PhysicalDeviceIDProperties PhysicalDeviceIDProperties;
    vk::PhysicalDeviceMemoryProperties PhysicalDeviceMemoryProperties;

  public:
    PhysicalDevice(const vk::PhysicalDevice &phDev);

    [[nodiscard]] auto GetDeviceUUID() const
    {
        return PhysicalDeviceIDProperties.deviceUUID;
    }
    [[nodiscard]] auto GetDriverUUID() const
    {
        return PhysicalDeviceIDProperties.driverUUID;
    }
    [[nodiscard]] std::string GetName() const
    {
        return PhysicalDeviceProperties.deviceName.data();
    }
    [[nodiscard]] vk::PhysicalDevice GetVkPhysicalDeviceHandle() const
    {
        return Handle;
    }
    [[nodiscard]] vk::SurfaceCapabilitiesKHR GetSurfaceCapabilities(vk::SurfaceKHR surface) const
    {
        return Handle.getSurfaceCapabilitiesKHR(surface).value;
    }
    const vk::PhysicalDevice *operator->() const
    {
        return &Handle;
    }
    vk::PhysicalDevice *operator->()
    {
        return &Handle;
    }

    [[nodiscard]] std::optional<VK::MemoryType> FindMemoryFor(vk::MemoryRequirements Requirements,const Flags<MemoryProperties>& Properties) const;
    
};
} // namespace VK

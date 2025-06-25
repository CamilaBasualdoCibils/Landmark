#include "PhysicalDevice.hpp"
#include <Graphics/Vulkan/Device.hpp>
namespace VK
{
    PhysicalDevice::PhysicalDevice(const vk::PhysicalDevice& phDev):Handle(phDev)
    {
        PhysicalDeviceProperties = phDev.getProperties();
        PhysicalDeviceProperties2.pNext = &PhysicalDeviceIDProperties;
        
        phDev.getProperties2(&PhysicalDeviceProperties2);

    }

} // namespace VK

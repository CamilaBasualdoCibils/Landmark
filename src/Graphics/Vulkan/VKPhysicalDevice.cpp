#include "VKPhysicalDevice.hpp"
#include "Graphics/Vulkan/Memory/VKMemory.hpp"
#include <Graphics/Vulkan/VKDevice.hpp>

VK::PhysicalDevice::PhysicalDevice(const vk::PhysicalDevice &phDev) : Handle(phDev)
{
    PhysicalDeviceProperties = phDev.getProperties();
    
    
    PhysicalDeviceIDProperties.pNext = &PhysicalDeviceDrmProperties;
    PhysicalDeviceProperties2.pNext = &PhysicalDeviceIDProperties;
    phDev.getProperties2(&PhysicalDeviceProperties2);
    PhysicalDeviceMemoryProperties = phDev.getMemoryProperties();
}

std::optional<VK::MemoryType> VK::PhysicalDevice::FindMemoryFor(vk::MemoryRequirements Requirements,
                                                                const Flags<MemoryProperties> &Properties) const
{
    for (int32_t i = 0; i < PhysicalDeviceMemoryProperties.memoryTypeCount; i++)
    {
        if ((Requirements.memoryTypeBits & (1 << i)) &&
            (Properties & PhysicalDeviceMemoryProperties.memoryTypes[i].propertyFlags) == Properties)
        {
            VK::MemoryType memType;
            memType.Index = i;
            memType.flags = PhysicalDeviceMemoryProperties.memoryTypes[i].propertyFlags;
            return memType;
        }
    }
    return std::nullopt;
}

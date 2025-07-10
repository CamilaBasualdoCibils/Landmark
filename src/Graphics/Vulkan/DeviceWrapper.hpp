#pragma once
#include "Graphics/Vulkan/Device.hpp"
#include "Graphics/Vulkan/Queue.hpp"

namespace VK
{
struct DeviceWrapperProperties
{
};
class DeviceWrapper
{
    GPURef<Device> DeviceHandle;
    GPURef<Queue> m_GraphicsQueue, m_PresentQueue, m_ComputeQueue;

  public:
    DeviceWrapper(GPURef<PhysicalDevice> phyDev,const DeviceWrapperProperties &Properties);
    [[nodiscard]] auto LogicalDevice() const {return DeviceHandle;}
    [[nodiscard]] auto GraphicsQueue() const {return m_GraphicsQueue;}
    [[nodiscard]] auto PresentQueue() const {return m_PresentQueue;}
    [[nodiscard]] auto ComputeQueue() const {return m_ComputeQueue;}
    
};
} // namespace VK
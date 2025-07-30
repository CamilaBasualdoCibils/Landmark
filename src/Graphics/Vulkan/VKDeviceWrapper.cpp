#include "VKDeviceWrapper.hpp"
#include "Graphics/Vulkan/VKDevice.hpp"
#include "Graphics/Vulkan/VKQueue.hpp"

VK::DeviceWrapper::DeviceWrapper(GPURef<PhysicalDevice> phyDev,const DeviceWrapperProperties &Properties)
{

    DeviceHandle.Make(phyDev,DeviceProperties{.Queues = {QueueSelection{.FamilyIndex = 0}}});

    m_GraphicsQueue.Make(QueueProperties{.Device = DeviceHandle, .QueueIndex = 0, .Family = 0});
    m_ComputeQueue = m_PresentQueue = m_GraphicsQueue;
}
#include "GPUPhysicalDevice.hpp"
#include <Graphics/Vulkan/Instance.hpp>
GPUPhysicalDevice::GPUPhysicalDevice( std::shared_ptr<VK::PhysicalDevice> _VKDevice,  std::shared_ptr<GL::Device>_GLDevice) : VKDevice(_VKDevice), GLDevice(_GLDevice), Name(VKDevice->GetName())
{
    LASSERT(std::memcpy(VKDevice->GetDeviceUUID().data(), GLDevice->DeviceUUID.data(), 16), "INVALID DEVICES");
}

GPUPhysicalDevice::~GPUPhysicalDevice()
{
}

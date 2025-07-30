#include "GPUInstance.hpp"
#include "Graphics/Vulkan/VKDeviceWrapper.hpp"
#include <Graphics/Vulkan/VKInstance.hpp>
#include <Graphics/OpenGL/EGL.hpp>
GPUInstance::GPUInstance(/* args */)
{
}

GPUInstance::~GPUInstance()
{
}

GPUInstance &GPUInstance::SetDevice(const std::shared_ptr<GPUPhysicalDevice> &PhysicalDevice)
{
    LASSERT(!Initialized, "INVALID USE");
    this->PhysicalDevice = PhysicalDevice;
    return *this;
}
GPUInstance &GPUInstance::Init()
{
    LASSERT(PhysicalDevice,"No Device Specified");
    VK::DeviceWrapperProperties DeviceProperties;
    VKDevice = std::make_shared<VK::DeviceWrapper>(PhysicalDevice->VK(),DeviceProperties); //Device->GetVulkanDevice().
    GLContext = GL::EGL::Get().CreateContext(PhysicalDevice->GL());
    return *this;
}
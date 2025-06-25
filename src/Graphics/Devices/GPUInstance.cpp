#include "GPUInstance.hpp"
#include <Graphics/Vulkan/Instance.hpp>
#include <Graphics/OpenGL/EGL.hpp>
GPUInstance::GPUInstance(/* args */)
{
}

GPUInstance::~GPUInstance()
{
}

GPUInstance &GPUInstance::SetDevice(const std::shared_ptr<GPUDevice> &Device)
{
    LASSERT(!Initialized, "INVALID USE");
    this->Device = Device;
    return *this;
}
GPUInstance &GPUInstance::Init()
{
    LASSERT(Device,"No Device Specified");
    VK::DeviceProperties DeviceProperties;
    VKDevice = std::make_shared<VK::Device>(Device->GetVulkanDevice(),DeviceProperties); //Device->GetVulkanDevice().
    GLContext = GL::EGL::Get().CreateContext(Device->GetOpenGLDevice());
    return *this;
}
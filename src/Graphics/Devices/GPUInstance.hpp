#pragma once
#include <pch.h>
#include <Graphics/Devices/GPUDevice.hpp>
#include <Graphics/Vulkan/Device.hpp>
#include <Graphics/OpenGL/EGL.hpp>
class GPUInstance
{
private:
    bool Initialized = false;
    std::shared_ptr<GPUDevice> Device;

    std::shared_ptr<VK::Device> VKDevice;
    std::shared_ptr<GL::Context> GLContext;

    /* data */
public:
    GPUInstance(/* args */);
    GPUInstance &SetDevice(const std::shared_ptr<GPUDevice> &Device);
    GPUInstance &Init();
    ~GPUInstance();

    [[nodiscard]] auto GetDevice() const {return Device;}
    [[nodiscard]] auto GetVulkanDevice() const {return VKDevice;}
    [[nodiscard]] auto GetOpenGLContext() const {return GLContext;}
};

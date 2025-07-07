#pragma once
#include "Graphics/Vulkan/DeviceWrapper.hpp"
#include <pch.h>
#include <Graphics/Devices/GPUPhysicalDevice.hpp>
#include <Graphics/OpenGL/EGL.hpp>
class GPUInstance
{
private:
    bool Initialized = false;
    std::shared_ptr<GPUPhysicalDevice> PhysicalDevice;

    std::shared_ptr<VK::DeviceWrapper> VKDevice;
    std::shared_ptr<GL::Context> GLContext;

    /* data */
public:
    GPUInstance(/* args */);
    GPUInstance &SetDevice(const std::shared_ptr<GPUPhysicalDevice> &Device);
    GPUInstance &Init();
    ~GPUInstance();

    [[nodiscard]] auto GetPhysicalDevice() const {return PhysicalDevice;}
    [[nodiscard]] auto VK() const {return VKDevice;}
    [[nodiscard]] auto GL() const {return GLContext;}
};

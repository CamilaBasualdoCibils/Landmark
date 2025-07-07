#pragma once
#include <pch.h>
#include <Graphics/Vulkan/PhysicalDevice.hpp>
#include <Graphics/OpenGL/EGL.hpp>
class GPUPhysicalDevice
{
public:
    struct VulkanProperties
    {
        vk::PhysicalDevice PhysicalDevice;
        vk::PhysicalDeviceProperties Properties;
        vk::PhysicalDeviceIDProperties IDProperties;
    } VulkanProperties;
    struct EGLProperties
    {
    };

private:
    std::shared_ptr<GL::Device> GLDevice;
    std::shared_ptr<VK::PhysicalDevice> VKDevice;
    const std::string Name;
    /* data */
public:
    GPUPhysicalDevice( std::shared_ptr<VK::PhysicalDevice> _VKDevice,  std::shared_ptr<GL::Device>_GLDevice);
    ~GPUPhysicalDevice();

    [[nodiscard]] const std::string &GetName() const { return Name; }
    [[nodiscard]] std::shared_ptr<VK::PhysicalDevice> VK() const {return VKDevice;}
    [[nodiscard]] std::shared_ptr<GL::Device> GL() const {return GLDevice;}
};

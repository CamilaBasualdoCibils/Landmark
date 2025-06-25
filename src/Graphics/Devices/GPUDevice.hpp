#pragma once
#include <pch.h>
#include <Graphics/Vulkan/PhysicalDevice.hpp>
#include <Graphics/OpenGL/EGL.hpp>
class GPUDevice
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
    GPUDevice( std::shared_ptr<VK::PhysicalDevice> _VKDevice,  std::shared_ptr<GL::Device>_GLDevice);
    ~GPUDevice();

    [[nodiscard]] const std::string &GetName() const { return Name; }
    [[nodiscard]] std::shared_ptr<VK::PhysicalDevice> GetVulkanDevice() const {return VKDevice;}
    [[nodiscard]] std::shared_ptr<GL::Device> GetOpenGLDevice() const {return GLDevice;}
};

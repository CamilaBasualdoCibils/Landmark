#pragma once
#include "Graphics/ICommandBuffer.hpp"
#include "Graphics/Synchronization.hpp"
#include "Graphics/Vulkan/Swapchain.hpp"
#include <GLFW/glfw3.h>
#include <pch.h>
#include <vulkan/vulkan_handles.hpp>
namespace Graphics {
class Window
{

    GLFWwindow *glfwWindowHandle;
    vk::SurfaceKHR vkSurface;
    GPURef<VK::Swapchain> Swapchain;

  public:
    Window();
    bool GetShouldClose() const
    {
        return glfwWindowShouldClose(glfwWindowHandle);
    }
    vk::SurfaceKHR GetVulkanSurface() const;

    GPURef<VK::Swapchain> GetSwapchain() const {return Swapchain;}
};
}

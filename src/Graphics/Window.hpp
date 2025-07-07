#pragma once
#include "Graphics/ICommandBuffer.hpp"
#include "Graphics/Synchronization.hpp"
#include "Graphics/Vulkan/Swapchain.hpp"
#include <GLFW/glfw3.h>
#include <pch.h>
#include <vulkan/vulkan_handles.hpp>
namespace Graphics
{
class Window
{

    GLFWwindow *glfwWindowHandle;
    vk::SurfaceKHR vkSurface;
    mutable GPURef<VK::Swapchain> Swapchain;

  public:
    Window();
    bool GetShouldClose() const
    {
        return glfwWindowShouldClose(glfwWindowHandle);
    }
    vk::SurfaceKHR GetVulkanSurface() const;

    GPURef<VK::Swapchain> GetSwapchain() const
    {
        if (!Swapchain)
            Swapchain.Make(vkSurface);
        return Swapchain;
    }
    GLFWwindow* GetGLFWHandle()const {return glfwWindowHandle;}
};
} // namespace Graphics

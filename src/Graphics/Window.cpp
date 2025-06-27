#include "Window.hpp"
#include "Graphics/GraphicsEngine.hpp"
#include "Graphics/ICommandBuffer.hpp"
#include "Graphics/Synchronization.hpp"
#include "Graphics/Vulkan/Instance.hpp"
#include "IO/GLFW/GLFW.hpp"
#include <cstdint>
#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan_handles.hpp>
Graphics::Window::Window()
{
    GLFW::Check();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, true);
    glfwWindowHint(GLFW_DECORATED, true);

    glfwWindowHandle = glfwCreateWindow(1920, 1080, "Default Name", nullptr, nullptr);
    if (!glfwWindowHandle)
        throw std::runtime_error("Failed to create glfw window");

    glfwSetWindowUserPointer(glfwWindowHandle, this);

    VkSurfaceKHR s;
    glfwCreateWindowSurface((VkInstance)(vk::Instance)VK::Instance::Get(), glfwWindowHandle, nullptr, &s);
    vkSurface = (vk::SurfaceKHR)s;


}

vk::SurfaceKHR Graphics::Window::GetVulkanSurface() const
{
    return vkSurface;
}

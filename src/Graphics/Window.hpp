#pragma once
#include "Graphics/Compositor/CompositeGroup.hpp"
#include "Graphics/Synchronization.hpp"
#include "Graphics/Vulkan/VKSwapchain.hpp"
#include <pch.h>
namespace Graphics
{
struct WindowProperties
{
    ivec2 InitialSize;
};
class Window
{

    GLFWwindow *glfwWindowHandle;
    vk::SurfaceKHR vkSurface;
    mutable GPURef<VK::Swapchain> Swapchain;
    std::shared_ptr<Graphics::CompositeGroup> WindowComposite;
    const std::string Name;
    GPURef<Graphics::Semaphore> BlitCompleteSemaphore = GPURef<Graphics::Semaphore>::MakeRef(),
                                ImageReadySemaphore = GPURef<Graphics::Semaphore>::MakeRef();
    ivec2 FramebufferSize;
    ivec2 WindowSize = {1920,1080};

  public:
    Window(const std::string &Name);
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
    void Render();
    void Present() const;
    GLFWwindow *GetGLFWHandle() const
    {
        return glfwWindowHandle;
    }
    std::shared_ptr<Graphics::CompositeGroup> GetCompositeGroup()
    {
        return WindowComposite;
    }
    std::shared_ptr<const Graphics::CompositeGroup> GetCompositeGroup() const
    {
        return WindowComposite;
    }
    vec2 GetContentScale() const;

  private:
    static void OnWindowResize(GLFWwindow *window, int width, int height)
    {
        reinterpret_cast<Window *>(glfwGetWindowUserPointer(window))->OnWindowResize(ivec2(width, height));
    }
    static void OnFramebufferResize(GLFWwindow *window, int width, int height)
    {
        reinterpret_cast<Window *>(glfwGetWindowUserPointer(window))->OnFramebufferResize(ivec2(width, height));
    }
    void OnWindowResize(ivec2 NewSize);
    void OnFramebufferResize(ivec2 NewSize);
};
} // namespace Graphics

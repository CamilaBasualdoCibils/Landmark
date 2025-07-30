#pragma once
#include "Graphics/Compositor/CompositeGroup.hpp"
#include "Graphics/Synchronization.hpp"
#include "Graphics/Vulkan/VKSwapchain.hpp"
#include <pch.h>
namespace Graphics
{
class Window
{

    GLFWwindow *glfwWindowHandle;
    vk::SurfaceKHR vkSurface;
    mutable GPURef<VK::Swapchain> Swapchain;
    std::shared_ptr<Graphics::CompositeGroup> WindowComposite;
    const std::string Name;
    GPURef<Graphics::Semaphore> BlitCompleteSemaphore = GPURef<Graphics::Semaphore>::MakeRef(),ImageReadySemaphore = GPURef<Graphics::Semaphore>::MakeRef();

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
};
} // namespace Graphics

#pragma once
#include "Graphics/Synchronization.hpp"
#include "Graphics/Vulkan/VKSwapchain.hpp"
#include "Render/Compositor/Pipeline.hpp"
#include <pch.h>
namespace Platform
{
struct WindowProperties
{
    ivec2 InitialSize;
};
class WindowManager;
class Window : public std::enable_shared_from_this<Window>
{
    friend WindowManager;
    GLFWwindow *glfwWindowHandle;
    vk::SurfaceKHR vkSurface;
    mutable GPURef<VK::Swapchain> Swapchain;
    // std::shared_ptr<Graphics::CompositeGroup> WindowComposite;
    std::shared_ptr<Render::Composite::Pipeline> CompositePipeline;
    const std::string Name;
    GPURef<Graphics::Semaphore> BlitCompleteSemaphore = GPURef<Graphics::Semaphore>::MakeRef(),
                                ImageReadySemaphore = GPURef<Graphics::Semaphore>::MakeRef();
    ivec2 FramebufferSize;
    ivec2 WindowSize = {1920, 1080};

  protected:
  public:
    Window(const std::string &Name);
    bool GetShouldClose() const
    {
        return glfwWindowShouldClose(glfwWindowHandle);
    }
    vk::SurfaceKHR GetVulkanSurface() const;

    GPURef<VK::Swapchain> GetSwapchain() const;

    void Present() const;
    GLFWwindow *GetGLFWHandle() const
    {
        return glfwWindowHandle;
    }

    std::shared_ptr<const Render::Composite::Pipeline> GetPipeline() const
    {
        return CompositePipeline;
    }
    std::shared_ptr<Render::Composite::Pipeline> GetPipeline()
    {
        return CompositePipeline;
    }

    vec2 GetContentScale() const;
    std::string_view GetName() const
    {
        return Name;
    }

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
} // namespace Platform

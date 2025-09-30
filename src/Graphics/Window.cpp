#include "Window.hpp"
#include "Graphics/Compositor/Compositor.hpp"
#include "Graphics/GraphicsEngine.hpp"
#include "Graphics/Synchronization.hpp"
#include "Graphics/Vulkan/VKInstance.hpp"
#include "IO/GLFW/GLFW.hpp"
#include <Graphics/Vulkan/Commands/CommandManager/VKImageCommands.hpp>
#include <Graphics/Vulkan/Commands/CommandManager/VKPipelineCommands.hpp>
Graphics::Window::Window(const std::string &Name) : Name(Name)
{
    GLFW::Check();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, true);
    glfwWindowHint(GLFW_DECORATED, true);
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, false);
    glfwWindowHint(GLFW_SCALE_FRAMEBUFFER, false);

    glfwWindowHandle = glfwCreateWindow(WindowSize.x, WindowSize.y, Name.c_str(), nullptr, nullptr);
    if (!glfwWindowHandle)
        throw std::runtime_error("Failed to create glfw window");

    glfwSetWindowUserPointer(glfwWindowHandle, this);
    glfwSetWindowSizeCallback(glfwWindowHandle, Window::OnWindowResize);
    glfwSetFramebufferSizeCallback(glfwWindowHandle, Window::OnFramebufferResize);

    glfwGetFramebufferSize(glfwWindowHandle, &FramebufferSize.x, &FramebufferSize.y);
    VkSurfaceKHR s;
    glfwCreateWindowSurface((VkInstance)(vk::Instance)VK::Instance::Get(), glfwWindowHandle, nullptr, &s);
    vkSurface = (vk::SurfaceKHR)s;

    WindowComposite = Graphics::Compositor::Get().MakeRootGroup(
        Graphics::CompositeGroupProperties{.Name = Name + "_WindowComposite", .ResolutionOverride = FramebufferSize});
}

vk::SurfaceKHR Graphics::Window::GetVulkanSurface() const
{
    return vkSurface;
}

void Graphics::Window::Present() const
{
    Swapchain->Present(GraphicsEngine::Get().GetMainGPU()->VK()->GraphicsQueue(), BlitCompleteSemaphore);

    while (Swapchain->IsOutOfDate())
    {
        Swapchain->Recreate(GetVulkanSurface());
    }
    // GraphicsEngine::Get().GetMainGPU()->VK()->LogicalDevice()->GetHandle().waitIdle();
}
vec2 Graphics::Window::GetContentScale() const
{
    vec2 scale;
    GLFWmonitor *monitor = glfwGetWindowMonitor(glfwWindowHandle);
    if (monitor)
    {
        glfwGetMonitorContentScale(monitor, &scale.x, &scale.y);
    }
    else
    {
        glfwGetWindowContentScale(glfwWindowHandle, &scale.x, &scale.y);
    }
    return scale;
}
void Graphics::Window::OnWindowResize(ivec2 NewSize)
{
    WindowSize = NewSize;
}
void Graphics::Window::OnFramebufferResize(ivec2 NewSize)
{
    //WindowComposite->SetResolutionOverride(NewSize);
    FramebufferSize = NewSize;
    //std::cerr << "Framebuffer Resize " << to_string(NewSize) << std::endl;
}
void Graphics::Window::Render()
{
    Graphics::CompositeContext context;

    GetSwapchain()->AcquireNextSwapchainImage(ImageReadySemaphore);

    while (GetSwapchain()->IsOutOfDate())
    {
        GetSwapchain()->Recreate(GetVulkanSurface());
        GetSwapchain()->AcquireNextSwapchainImage(ImageReadySemaphore);
        WindowComposite->SetResolutionOverride(GetSwapchain()->GetExtent());
        std::cerr << "New Res " << to_string(GetSwapchain()->GetExtent()) << std::endl;
    }

    std::shared_ptr<VK::CommandManager> CopyToSwapCmds = std::make_shared<VK::CommandManager>();
    CopyToSwapCmds->PushGroupLabel("Cpy to Swapchain");
    CopyToSwapCmds->WaitSemaphore(ImageReadySemaphore);
    CopyToSwapCmds->Push(VK::TransitionImageLayoutCommand(
        Swapchain->GetCurrentImage().imageRaw, VK::ImageLayouts::ePresentSrc, VK::ImageLayouts::eTransferDst,
        VK::PipelineStageFlags::eBottomOfPipe, VK::PipelineStageFlags::eTransfer));
        
    CopyToSwapCmds->Push(VK::BlitImageCommand(WindowComposite->GetAttachments().at("Main"),
                                              Swapchain->GetCurrentImage().imageRaw, VK::ImageLayouts::eGeneral,
                                              VK::ImageLayouts::eGeneral, VK::Filter::eNearest, ivec3(0), ivec3(0),
                                              ivec3(WindowComposite->GetResolution(), 1)));
    CopyToSwapCmds->Push(VK::TransitionImageLayoutCommand(
        Swapchain->GetCurrentImage().imageRaw, VK::ImageLayouts::eTransferDst, VK::ImageLayouts::ePresentSrc,
        VK::PipelineStageFlags::eTransfer, VK::PipelineStageFlags::eBottomOfPipe));

    CopyToSwapCmds->PopGroupLabel();
    CopyToSwapCmds->SignalSemaphore(BlitCompleteSemaphore);
    GraphicsEngine::Get().Push({CopyToSwapCmds});
}

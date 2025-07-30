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

    glfwWindowHandle = glfwCreateWindow(1920, 1080, Name.c_str(), nullptr, nullptr);
    if (!glfwWindowHandle)
        throw std::runtime_error("Failed to create glfw window");

    glfwSetWindowUserPointer(glfwWindowHandle, this);

    VkSurfaceKHR s;
    glfwCreateWindowSurface((VkInstance)(vk::Instance)VK::Instance::Get(), glfwWindowHandle, nullptr, &s);
    vkSurface = (vk::SurfaceKHR)s;

    WindowComposite = Graphics::Compositor::Get().MakeRootGroup(
        Graphics::CompositeGroupProperties{.Name = Name + "_WindowComposite", .ResolutionOverride = uvec2(1920, 1080)});
}

vk::SurfaceKHR Graphics::Window::GetVulkanSurface() const
{
    return vkSurface;
}

void Graphics::Window::Present() const
{
    GraphicsEngine::Get().GetMainWindow()->GetSwapchain()->Present(
        GraphicsEngine::Get().GetMainGPU()->VK()->GraphicsQueue(), BlitCompleteSemaphore);
    //GraphicsEngine::Get().GetMainGPU()->VK()->LogicalDevice()->GetHandle().waitIdle();
}
void Graphics::Window::Render()
{
    Graphics::CompositeContext context;
    GetSwapchain()->AcquireNextSwapchainImage(ImageReadySemaphore);

    std::shared_ptr<VK::CommandManager> CopyToSwapCmds = std::make_shared<VK::CommandManager>();
    CopyToSwapCmds->PushGroupLabel("Cpy to Swapchain");
    CopyToSwapCmds->WaitSemaphore(ImageReadySemaphore);
    CopyToSwapCmds->Push(VK::TransitionImageLayoutCommand(
        Swapchain->GetCurrentImage().imageRaw, VK::ImageLayouts::ePresentSrc, VK::ImageLayouts::eTransferDst,
        VK::PipelineStageFlags::eBottomOfPipe, VK::PipelineStageFlags::eTransfer));
    CopyToSwapCmds->Push(VK::BlitImageCommand(WindowComposite->GetAttachments().at("Main"), Swapchain->GetCurrentImage().imageRaw,
                                              VK::ImageLayouts::eGeneral, VK::ImageLayouts::eGeneral,
                                              VK::Filter::eNearest, ivec3(0), ivec3(0),
                                              ivec3(WindowComposite->GetResolution(), 1)));
    CopyToSwapCmds->Push(VK::TransitionImageLayoutCommand(
        Swapchain->GetCurrentImage().imageRaw, VK::ImageLayouts::eTransferDst, VK::ImageLayouts::ePresentSrc,
        VK::PipelineStageFlags::eTransfer, VK::PipelineStageFlags::eBottomOfPipe));

    CopyToSwapCmds->PopGroupLabel();
    CopyToSwapCmds->SignalSemaphore(BlitCompleteSemaphore);
    GraphicsEngine::Get().Push({CopyToSwapCmds});
}

#include "Window.hpp"
#include "Graphics/GraphicsEngine.hpp"
#include "Graphics/Synchronization.hpp"
#include "Graphics/Vulkan/VKInstance.hpp"
#include "IO/GLFW/GLFW.hpp"
#include "Platform/Window/WindowImGuiLayer.hpp"
#include "Render/Compositor/Compositor.hpp"
#include <Graphics/Vulkan/Commands/CommandManager/VKImageCommands.hpp>
#include <Graphics/Vulkan/Commands/CommandManager/VKPipelineCommands.hpp>

Platform::Window::Window(const std::string &Name) : Name(Name)
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

    Render::Composite::Layout layout;
    layout.PushColorAttachment(Render::Composite::ColorAttachmentInfo{.Name = "Color",
                                                                      .Usage = VK::ImageUsage::eColorAttachment,
                                                                      .format = VK::Format::eRGBA8_UNorm,
                                                                      .clearPolicy = VK::AttachmentLoadOp::eClear});
    layout.Build();
    CompositePipeline = Render::Compositor::Get().MakeBasePipeline(
        Name + "_WindowComposite",
        Render::Composite::PipelineProperties{
            .settings = Render::Composite::PipelineSettings{.BaseResolution = FramebufferSize},
            .layout = layout,
        });
    CompositePipeline->Push<WindowImGuiLayer>();
    // CompositePipeline->Build();
    //  WindowComposite = Graphics::Compositor::Get().MakeRootGroup(
    //      Graphics::CompositeGroupProperties{.Name = Name + "_WindowComposite", .ResolutionOverride =
    //      FramebufferSize});
}

vk::SurfaceKHR Platform::Window::GetVulkanSurface() const
{
    return vkSurface;
}

GPURef<VK::Swapchain> Platform::Window::GetSwapchain() const
{
    if (!Swapchain)
        Swapchain.Make(vkSurface);
    return Swapchain;
}

void Platform::Window::Present() const
{
    GetSwapchain()->AcquireNextSwapchainImage(ImageReadySemaphore);

    while (GetSwapchain()->IsOutOfDate())
    {
        GetSwapchain()->Recreate(GetVulkanSurface());
        GetSwapchain()->AcquireNextSwapchainImage(ImageReadySemaphore);
        auto OldSettings = CompositePipeline->GetEntrySettings();
        OldSettings.BaseResolution = GetSwapchain()->GetExtent();
        CompositePipeline->SetEntrySettings(OldSettings, true);
        // WindowComposite->SetResolutionOverride(GetSwapchain()->GetExtent());
        // std::cerr << "New Res " << to_string(GetSwapchain()->GetExtent()) << std::endl;
    }
    LASSERT(CompositePipeline->IsBuilt(), "Fuck?");
    LASSERT(!CompositePipeline->GetColorImages().empty(), "Shit?");

    std::shared_ptr<VK::CommandManager> CopyToSwapCmds = std::make_shared<VK::CommandManager>();
    CopyToSwapCmds->PushGroupLabel("Cpy to Swapchain");

    CopyToSwapCmds->WaitSemaphore(ImageReadySemaphore); // wait for image to be ready

    // Get the source texture and transition it to transfersrc
    auto SourceTexture = CompositePipeline->GetColorImages().front().Texture;
    // CopyToSwapCmds->Push(
    //     VK::TransitionImageLayoutCommand(SourceTexture, VK::ImageLayouts::eGeneral, VK::ImageLayouts::eTransferSrc,
    //                                      VK::PipelineStageFlags::eAllGraphics, VK::PipelineStageFlags::eTransfer));

    // transition swapchain image to transfer dest
    CopyToSwapCmds->Push(VK::TransitionImageLayoutCommand(
        Swapchain->GetCurrentImage().imageRaw, VK::ImageLayouts::ePresentSrc, VK::ImageLayouts::eTransferDst,
        VK::PipelineStageFlags::eBottomOfPipe, VK::PipelineStageFlags::eTransfer));
    // copy contents
    CopyToSwapCmds->Push(VK::BlitImageCommand(
        SourceTexture, Swapchain->GetCurrentImage().imageRaw, VK::ImageLayouts::eGeneral, VK::ImageLayouts::eGeneral,
        VK::Filter::eNearest, ivec3(0), ivec3(0), ivec3(GetSwapchain()->GetExtent(), 1)));
    // Transition swapchain to present
    CopyToSwapCmds->Push(VK::TransitionImageLayoutCommand(
        Swapchain->GetCurrentImage().imageRaw, VK::ImageLayouts::eTransferDst, VK::ImageLayouts::ePresentSrc,
        VK::PipelineStageFlags::eTransfer, VK::PipelineStageFlags::eBottomOfPipe));
    //
    CopyToSwapCmds->SignalSemaphore(BlitCompleteSemaphore);
    CopyToSwapCmds->PopGroupLabel();
    GraphicsEngine::Get().ExecuteNow({CopyToSwapCmds});

    Swapchain->Present(GraphicsEngine::Get().GetMainGPU()->VK()->GraphicsQueue(), BlitCompleteSemaphore);

    // while (Swapchain->IsOutOfDate())
    //{
    //     Swapchain->Recreate(GetVulkanSurface());
    // }
    //GraphicsEngine::Get().GetMainGPU()->VK()->LogicalDevice()->GetHandle().waitIdle();
}
vec2 Platform::Window::GetContentScale() const
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
void Platform::Window::OnWindowResize(ivec2 NewSize)
{
    WindowSize = NewSize;
}
void Platform::Window::OnFramebufferResize(ivec2 NewSize)
{
    // WindowComposite->SetResolutionOverride(NewSize);
    FramebufferSize = NewSize;
    // std::cerr << "Framebuffer Resize " << to_string(NewSize) << std::endl;
}
/*
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
*/
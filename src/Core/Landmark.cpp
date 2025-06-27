#include "Landmark.hpp"
#include "Graphics/ICommandBuffer.hpp"
#include "Graphics/OpenGL/CommandBuffer/DrawCommands.hpp"
#include "Graphics/OpenGL/CommandBuffer/TextureCommands.hpp"
#include "Graphics/Synchronization.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Vulkan/CommandBuffer/CommandBuffer.hpp"
#include "Graphics/Vulkan/CommandBuffer/ImageCommands.hpp"
#include "Graphics/Vulkan/CommandBuffer/PipelineCommands.hpp"
#include "Graphics/Vulkan/Images/ImageEnums.hpp"
#include "Graphics/Vulkan/Pipeline/PipelineEnums.hpp"
#include "Graphics/Vulkan/Rendering/Framebuffer.hpp"
#include "Graphics/Vulkan/Rendering/RenderPass.hpp"
#include "Graphics/Vulkan/Swapchain.hpp"
#include "pch.h"
#include <GL/gl.h>
#include <GL/glext.h>
#include <Graphics/Devices/GPUSelector.hpp>
#include <Graphics/GraphicsEngine.hpp>
#include <Graphics/Vulkan/Instance.hpp>
#include <glm/fwd.hpp>
#include <memory>
#include <vulkan/vulkan_handles.hpp>
Landmark::Landmark(const LandmarkStartProperties &Properties)
{

    GraphicsEngineProperties props;
    GraphicsEngine::Make(props);

    VK::RenderPassProperties RenderPassProps{
        .Attachments = {VK::RenderPassAttachment{
            .InitialLayout = VK::ImageLayouts::eColorAttachment,
            .FinalLayout = VK::ImageLayouts::ePresentSrc,
            .Format = VK::ImageFormats::eRGBA8_UNorm,
        }},
        .Subpasses = {
            VK::RenderPassSubpass{.BindPoint = VK::PipelineBindPoint::eGraphics,
                                  .SubpassAttachments = {VK::RenderPassSubpassAttachment{
                                      .AttachmentID = 0, .IdealLayout = VK::ImageLayouts::eColorAttachment}}}}};

    GPURef<VK::RenderPass> renderPass;
    renderPass.Make(RenderPassProps);

    VK::FramebufferProperties FramebufferProperties{
        .Dimensions = {512, 512}, .LayerCount = 1, .Renderpass = renderPass};
    VK::Framebuffer framebuffer(FramebufferProperties);

    Graphics::TextureProperties TextureProps{
        .Dimensions = {512, 512, 1}, .Levels = 1, .Format = Graphics::TextureFormatValues::eBGRA8_UNorm};

    GPURef<Graphics::Texture> TestTexture = GPURef<Graphics::Texture>::MakeRef(TextureProps);

    GPURef<Graphics::Semaphore> ImageReadySemaphore = GPURef<Graphics::Semaphore>::MakeRef(),
                                RenderingFinishedSemaphore = GPURef<Graphics::Semaphore>::MakeRef();
    VK::CommandBuffer vulkan;

    auto openGL = GraphicsEngine::Get().MakeGLCommandBuffer();
    while (!GraphicsEngine::Get().GetMainWindow()->GetShouldClose())
    {
        GraphicsEngine::Get().GetMainWindow()->GetSwapchain()->AcquireNextSwapchainImage(ImageReadySemaphore);
        vulkan.Clear();
        openGL->Clear();

        vulkan.WaitSemaphore(ImageReadySemaphore); // wait until monitor is ready

        // Transition swapchain to be transfer dst
        vulkan.Push<VK::TransitionImageLayout>(
            GraphicsEngine::Get().GetMainWindow()->GetSwapchain()->GetCurrentImage().imageRaw,
            VK::ImageLayouts::eUndefined, VK::ImageLayouts::eTransferDst, VK::PipelineStageFlags::eTopOfPipe,
            VK::PipelineStageFlags::eTransfer);

        // transfer test texture to be transfer dst
        vulkan.Push<VK::TransitionImageLayout>(TestTexture, VK::ImageLayouts::eGeneral, VK::ImageLayouts::eTransferDst,
                                               VK::PipelineStageFlags::eAllCommands, VK::PipelineStageFlags::eTransfer);

        // clear testTexture to green and transition from tranfer dst -> src
        vulkan.Push<VK::ClearColorImage>(TestTexture, VK::ImageLayouts::eTransferDst, vec4{0, 1, 0, 1});
        vulkan.Push<VK::TransitionImageLayout>(TestTexture, VK::ImageLayouts::eTransferDst,
                                               VK::ImageLayouts::eTransferSrc, VK::PipelineStageFlags::eTransfer,
                                               VK::PipelineStageFlags::eTransfer);

        openGL->WaitSemaphore(vulkan.SignalSemaphore()); // OpenGl wait for vulkan
        // Clear the top half of the texture to red
        openGL->Push<GL::ClearTextureImage>(TestTexture, uvec4(255, 0, 0, 1), uvec3(0, 0, 0), uvec3(512, 256, 1));

        vulkan.WaitSemaphore(openGL->SignalSemaphore()); // Vulkan wait for OpenGL

        // Copy contents of Test Texture to monitor image and transition so it is in presentSrc Format
        vulkan.Push<VK::CopyImage>(TestTexture,
                                   GraphicsEngine::Get().GetMainWindow()->GetSwapchain()->GetCurrentImage().imageRaw,
                                   VK::ImageLayouts::eTransferSrc, VK::ImageLayouts::eTransferDst, ivec3{0, 0, 0},
                                   ivec3{0, 0, 0}, ivec3{512, 512, 0});
        vulkan.Push<VK::TransitionImageLayout>(TestTexture, VK::ImageLayouts::eTransferSrc,
                                               VK::ImageLayouts::ePresentSrc, VK::PipelineStageFlags::eTransfer,
                                               VK::PipelineStageFlags::eBottomOfPipe);
        // met me know when youre done
        vulkan.SignalSemaphore(RenderingFinishedSemaphore);

        // Submit OpenGL and Vulkan for execution
        vulkan.Submit(GraphicsEngine::Get().GetMainGPU()->GetVulkanDevice()->GetGraphicsQueue());
        GraphicsEngine::Get().Execute({openGL});

        // Present Image once Rendering has finished
        GraphicsEngine::Get().GetMainWindow()->GetSwapchain()->Present(
            GraphicsEngine::Get().GetMainGPU()->GetVulkanDevice()->GetGraphicsQueue(), RenderingFinishedSemaphore);

        GraphicsEngine::Get().Update();
        GraphicsEngine::Get().GetMainGPU()->GetVulkanDevice()->GetHandle().waitIdle();
    }
}

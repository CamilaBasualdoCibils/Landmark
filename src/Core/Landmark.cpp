#include "Landmark.hpp"
#include "Graphics/ICommandBuffer.hpp"
#include "Graphics/OpenGL/CommandBuffer/DrawCommands.hpp"
#include "Graphics/OpenGL/CommandBuffer/TextureCommands.hpp"
#include "Graphics/Synchronization.hpp"
#include "Graphics/Texture.hpp"
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
#include <memory>
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
        .Dimensions = {512, 512, 1}, .Levels = 1, .Format = Graphics::TextureFormatValues::eRGBA8_UNorm};

    GPURef<Graphics::Texture> TestTexture = GPURef<Graphics::Texture>::MakeRef(TextureProps);

    GPURef<Graphics::Semaphore> ImageReadySemaphore;
    ImageReadySemaphore.Make();

    auto GLCommandBuffer = GraphicsEngine::Get().MakeGLCommandBuffer();
    GLCommandBuffer->WaitSemaphore(ImageReadySemaphore);
    GLCommandBuffer->Push<GL::DrawArrays>(GL_TRIANGLES, 0, 3);
    GLCommandBuffer->Push<GL::DrawArrays>(GL_TRIANGLES, 5, 16);

    GLCommandBuffer->Push<GL::BindTexture>(0, TestTexture);

    // GLCommandBuffer->WaitSemaphore(testSemaphore);

    // GLCommandBuffer->Push<GL::DrawArrays>(GL_VERTEX_SHADER,0,3);

    while (!GraphicsEngine::Get().GetMainWindow()->GetShouldClose())
    {
        GraphicsEngine::Get().GetMainWindow()->GetSwapchain()->AcquireNextSwapchainImage(ImageReadySemaphore);
        GraphicsEngine::Get().Execute({GLCommandBuffer});

        GraphicsEngine::Get().GetMainGPU()->GetVulkanDevice().;
        GraphicsEngine::Get().Update();
    }
}

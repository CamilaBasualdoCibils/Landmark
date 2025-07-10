#include "Framebuffer.hpp"
#include "Graphics/GraphicsEngine.hpp"
#include "pch.h"
#include <vulkan/vulkan_enums.hpp>
#include <vulkan/vulkan_structs.hpp>

VK::Framebuffer::Framebuffer(const VK::FramebufferProperties &Properties)
{
    vk::FramebufferCreateInfo CreateInfo;
    CreateInfo.width = Properties.Dimensions.x;
    CreateInfo.height = Properties.Dimensions.y;

    // Imageless allows the ImageViews to be provided at the BeginRenderpass() stage which is more OpenGL like
    // https://docs.vulkan.org/guide/latest/extensions/VK_KHR_imageless_framebuffer.html
    CreateInfo.flags = vk::FramebufferCreateFlagBits::eImageless;
    CreateInfo.layers = Properties.LayerCount;
    CreateInfo.renderPass = *Properties.Renderpass;
    CreateInfo.attachmentCount = Properties.Renderpass->GetProperties().Attachments.size();
    std::vector<vk::FramebufferAttachmentImageInfo> ImageInfos;
    vk::Format format;
    for (int i = 0; i < Properties.Renderpass->GetProperties().Attachments.size();i++)
    {
        const auto Attachment =Properties.Renderpass->GetProperties().Attachments[i];
        vk::FramebufferAttachmentImageInfo ImageInfo;
        ImageInfo.width = Properties.Dimensions.x;
        ImageInfo.height = Properties.Dimensions.y;
        ImageInfo.layerCount = Properties.LayerCount;
        ImageInfo.pViewFormats = &format;
        ImageInfo.viewFormatCount = 1;
        format = (vk::Format)Attachment.format; 
        ImageInfo.usage = vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eSampled; 
        ImageInfos.push_back(ImageInfo);
    }
    vk::FramebufferAttachmentsCreateInfo AttachCreateInfo;
    AttachCreateInfo.setAttachmentImageInfos(ImageInfos);
    CreateInfo.pNext = & AttachCreateInfo;
    const auto CreateResult =
        GraphicsEngine::Get().GetMainGPU()->VK()->LogicalDevice()->GetHandle().createFramebuffer(CreateInfo);
    LASSERT(CreateResult.result == vk::Result::eSuccess, "dang");

    Handle = CreateResult.value;
}
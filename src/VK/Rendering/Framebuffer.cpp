#include "Framebuffer.h"
#include <VK/Rendering/RenderPass.h>
#include <VK/Images/CombinedImageSampler.h>
Framebuffer::Framebuffer(const FramebufferProperties &prop, const RenderPass &rp) : properties(prop)
{
    CreateImages();
    CreateFramebuffer(rp);
}

Framebuffer::Framebuffer(const std::vector<CombinedImageSampler> &_images, const RenderPass &rp)
{
    
    images = _images;
    properties.dimensions = images[0].GetImage().GetProperties().dimensions;
    for (const auto& att: images) {
        FramebufferAttachmentProperties attDesc;
        attDesc.format = att.GetImage().GetProperties().format;
        properties.attachments.push_back(attDesc);
    }
    CreateFramebuffer(rp);
}

void Framebuffer::Destroy()
{
    GetvkDevice().destroyFramebuffer(framebuffer);
    for (auto &im : images)
        im.Destroy();
}

void Framebuffer::CreateFramebuffer(const RenderPass &rp)
{
    if (framebuffer != VK_NULL_HANDLE)
    {
        Destroy();
    }


    std::vector<vk::ImageView> imageViews;
    for (const auto &image : images)
        imageViews.push_back(image.GetImageView().GetVkImageView());

    vk::FramebufferCreateInfo framebuffer_create_info;
    framebuffer_create_info.pAttachments = imageViews.data();
    framebuffer_create_info.attachmentCount = imageViews.size();
    framebuffer_create_info.width = properties.dimensions.x;
    framebuffer_create_info.height = properties.dimensions.y;
    framebuffer_create_info.renderPass = rp.GetVkRenderPass();
    framebuffer_create_info.layers = properties.dimensions.z;

    framebuffer = GetvkDevice().createFramebuffer(framebuffer_create_info).value;
}

void Framebuffer::CreateImages()
{
    images.reserve(properties.attachments.size());
    for (const auto &at : properties.attachments)
    {
        CombinedImageSampler::CISProperties cisProp;
        cisProp.image.format = at.format;
        cisProp.image.memoryTilling = at.tilling;
        cisProp.image.dimensions = properties.dimensions;
        cisProp.image.layers = 1;
        cisProp.image.mipLevels = 1;
        cisProp.image.Samples = 1;
        cisProp.image.usage = Flags<ImageUsage>(ImageUsage::COLOR_ATTACHMENT);

        images.push_back(cisProp);
    }
}

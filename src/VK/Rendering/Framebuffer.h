#pragma once
#include <pch.h>
#include <VK/Images/CombinedImageSampler.h>
#include <VK/Devices/DeviceResource.h>
//class CombinedImageSampler;
class RenderPass;
class Framebuffer : public DeviceResource
{
public:
    struct FramebufferAttachmentProperties
    {
        ColorFormat format = ColorFormat::RGBA8SRGB;
        MemoryTillingType tilling = MemoryTillingType::OPTIMAL;
    };
    struct FramebufferProperties
    {
        uvec3 dimensions = {0, 0, 1};
        std::vector<FramebufferAttachmentProperties> attachments;
    };

private:
    vk::Framebuffer framebuffer;
    std::vector<CombinedImageSampler> images;
    FramebufferProperties properties;

public:
    Framebuffer(const FramebufferProperties &prop, const RenderPass &rp);
    //creates a framebuffer from existing images
    Framebuffer(const std::vector<CombinedImageSampler> &_images, const RenderPass &rp);
    void Destroy() override;
    vk::Framebuffer GetVkFramebuffer() const {return framebuffer;}
    
private:
    void CreateFramebuffer(const RenderPass &rp);
    void CreateImages();
};
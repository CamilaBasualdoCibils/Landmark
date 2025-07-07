#pragma once
#include "Graphics/Vulkan/Images/Texture.hpp"
#include <pch.h>
#include <vulkan/vulkan_handles.hpp>
#include <vulkan/vulkan_structs.hpp>

// Render target is the equivalent of OpenGL framebuffers for the use of Dynamic Rendering. Core in vulkan as of 1.3
namespace VK
{
class RenderTarget
{
    std::array<GPURef<VK::Texture>, 8> ColorAttachments;
    GPURef<VK::Texture> DepthAttachment, StencilAttachment;
    bool dirty = true;
    
    std::array<vk::RenderingAttachmentInfo,8> ColorAttachmentInfos;
    vk::RenderingInfoKHR RenderingInfo;
    uvec2 ViewportOffset,ViewportSize;
  public:
    RenderTarget();
    RenderTarget &AttachColor(uint32_t Index, GPURef<VK::Texture> Attachment)
    {
        LASSERT(Index >= 0 && Index < 8, "Damn");
        LASSERT(Attachment, "what");

        ColorAttachments[Index] = Attachment;
        dirty = true;
        return *this;
    }
    RenderTarget &DetachColor(uint32_t Index)
    {
        LASSERT(Index >= 0 && Index < 8, "Damn");
        LASSERT(!ColorAttachments[Index], "what");
        ColorAttachments[Index] = nullptr;
        return *this;
    }
    RenderTarget& SetViewport(uvec2 Offset,uvec2 Size)
    {
        ViewportOffset = Offset;
        ViewportSize = Size;
        return *this;
    }
    RenderTarget &AttachDepth(GPURef<VK::Texture> Attachment);
    RenderTarget &AttachStencil(GPURef<VK::Texture> Attachment);
    RenderTarget &AttachDepthStencil(GPURef<VK::Texture> Attachment);
    
};
} // namespace VK
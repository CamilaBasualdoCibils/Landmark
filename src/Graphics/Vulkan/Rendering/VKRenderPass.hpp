#pragma once
#include <pch.h>
#include "Graphics/Vulkan/VKEnums.hpp"
namespace VK
{
struct RenderPassAttachment
{
    AttachmentLoadOp LoadOp = AttachmentLoadOp::eLoad;
    AttachmentStoreOp StoreOp = AttachmentStoreOp::eStore;
    AttachmentLoadOp StencilLoadOp = AttachmentLoadOp::eLoad;
    AttachmentStoreOp StencilStoreOp = AttachmentStoreOp::eStore;
    ImageLayouts InitialLayout = ImageLayouts::eUndefined;
    ImageLayouts FinalLayout = ImageLayouts::ePresentSrc;
    Format format = Format::eRGBA8_UNorm;
};
struct RenderPassSubpassAttachment
{
    uint32_t AttachmentID;
    ImageLayouts IdealLayout;
};
struct RenderPassSubpass
{
    PipelineBindPoint BindPoint = PipelineBindPoint::eGraphics;
    std::vector<RenderPassSubpassAttachment> SubpassAttachments;
};
struct RenderPassProperties
{
    std::vector<RenderPassAttachment> Attachments;
    std::vector<RenderPassSubpass> Subpasses;
};
class RenderPass
{
    vk::RenderPass Handle;
    RenderPassProperties Properties;

  public:
    RenderPass(const RenderPassProperties &Properties);
    operator vk::RenderPass() const
    {
        return Handle;
    }
    const RenderPassProperties &GetProperties() const
    {
        return Properties;
    }
    [[nodiscard]] vk::RenderPass GetHandle() const
    {
        return Handle;
    }
};
} // namespace VK
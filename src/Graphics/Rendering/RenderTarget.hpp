#pragma once

#include "Graphics/Interop/Interop.hpp"
#include "Graphics/OpenGL/Rendering/Framebuffer.hpp"
#include "Graphics/Pipelines/PipelineEnums.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Vulkan/Rendering/RenderTarget.hpp"
#include <pch.h>

namespace Graphics
{
struct RenderSubTargetAttachments
{
    uint32_t AttachmentID;
    TextureLayout IdealLayout;
};
struct RenderSubTarget
{
    PipelineBindPoint BindPoint = PipelineBindPoint::eGraphics;
    std::vector<RenderSubTargetAttachments> Attachments;
};
struct RenderTargetAttachments
{
    TextureLayout LayoutBefore, LayoutAfter;
    TextureFormat Format;
};
struct RenderTargetProperties
{
    std::vector<RenderTargetAttachments> Attachments;
    std::vector<RenderSubTarget> SubTargets;
};
class RenderTarget : public InteropObject<VK::RenderTarget, GL::Framebuffer, InteropTransactionType::eNone>
{
    const RenderTargetProperties Properties;

  public:
    RenderTarget(const RenderTargetProperties &Properties);
};
} // namespace Graphics

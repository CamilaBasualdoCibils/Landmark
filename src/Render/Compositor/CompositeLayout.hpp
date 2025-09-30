
#pragma once
#include <Graphics/Vulkan/Images/VKTexture.hpp>
#include <Graphics/Vulkan/Rendering/VKRenderTarget.hpp>
#include <pch.h>
namespace Render
{
enum class CompositeAttachmentUsage
{
    eColor,
    eDepth,
    eStencil,
    eDepthStencil,
    eTransferSrc,
    eTransferDst,
    eTransfer
};
struct ColorAttachmentInfo
{
};
struct DepthStencilAttachmentInfo
{
};
class CompositeLayout
{

    std::vector<ColorAttachmentInfo> ColorAttachments;
    std::optional<DepthStencilAttachmentInfo> DepthStencilAttachment;
    bool Built = false;

  public:
    void PushColorAttachment(const ColorAttachmentInfo &)
    {
    }
    void PushDepthStencilAttachment(const DepthStencilAttachmentInfo &)
    {
    }
    void Build() { LASSERT(!Built, "Layout already built");built = true; };
};

} // namespace Render

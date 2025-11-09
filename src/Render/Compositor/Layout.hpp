
#pragma once
#include "Attachment.hpp"
#include <pch.h>
namespace Render
{
namespace Composite
{

class Layout
{
    boost::container::static_vector<ColorAttachmentInfo, MAX_COLOR_ATTACHMENTS> ColorAttachments;
    std::optional<DepthStencilAttachmentInfo> DepthStencilAttachment;
    bool Built = false;

  public:
    void PushColorAttachment(const ColorAttachmentInfo &ca)
    {
        LASSERT(ColorAttachments.size() != ColorAttachments.capacity(), "Full of attachments");
        ColorAttachments.push_back(ca);
    }
    void SetDepthStencilAttachment(const DepthStencilAttachmentInfo &Da)
    {
        DepthStencilAttachment = Da;
    }
    void Build()
    {
        LASSERT(!Built, "Layout already built");
        Built = true;
    };
    const decltype(ColorAttachments) &GetColorAttachments() const
    {
        return ColorAttachments;
    }
    const decltype(DepthStencilAttachment) &GetDepthStencilAttachment() const
    {
        return DepthStencilAttachment;
    }
};
} // namespace Composite

} // namespace Render

#pragma once
#include "Render/Compositor/Pass.hpp"
#include <pch.h>
namespace Render
{

const static inline Composite::ColorAttachmentInfo DeferredColorAttachments[] = {Composite::ColorAttachmentInfo{
    .Name = "Albedo",
    .Usage = {VK::ImageUsage::eColorAttachment},
    .format = VK::Format::eRGBA8_UNorm,
    .clearPolicy = VK::AttachmentLoadOp::eClear,
    .ClearValueColor = vec4(0),
}};
class DeferredRenderPass : public Composite::Pass
{

    // mutable std::optional<Composite::Layout> DeferredPassLayout;

  public:
    DeferredRenderPass(std::string_view name);
    static const Composite::Layout &GetDeferredLayout()
    {
        static std::optional<Composite::Layout> DeferredPassLayout;
        if (!DeferredPassLayout.has_value())
        {
            DeferredPassLayout.emplace();
            for (const auto &ColorAttachment : DeferredColorAttachments)
            {
                DeferredPassLayout->PushColorAttachment(ColorAttachment);
            }
        }

        return *DeferredPassLayout;
    };
};
} // namespace Render
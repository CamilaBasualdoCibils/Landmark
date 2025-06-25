//
// Created by camila on 10/29/24.
//

#include "Frame.h"
#include "Act.hpp"
Frame::Frame(const frame_properties &_prop, Act &act) : properties(_prop)
{

    Framebuffer::FramebufferProperties fb_prop;
    const RenderPass::Renderpass_Properties &rp_prop = act.getRenderPass().getProperties();
    const auto &subpass_attachments = rp_prop.subpasses[_prop.lens_subpass_id];
    for (int i = 0; i < subpass_attachments.color_attachments.size(); i++)
    {
        Framebuffer::FramebufferAttachmentProperties attachment;
        attachment.format = rp_prop.color_attachments[subpass_attachments.color_attachments[i].attachment].format;
        attachment.tilling = MemoryTillingType::OPTIMAL;
        attachment.usages = {ImageUsage::COLOR_ATTACHMENT,ImageUsage::SAMPLED}; //COULD AFFECT PERFORMANCE. << SAMPLED HARD CODED
        fb_prop.attachments.push_back(attachment);
    }

    fb_prop.dimensions = properties.dimensions;
    framebuffer.emplace(fb_prop,act.getRenderPass());
}

Frame::~Frame()
{
    if (framebuffer.has_value())framebuffer->Destroy();
    logger.Debug("frame destroyed");
}

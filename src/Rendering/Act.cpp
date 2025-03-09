#include "Act.hpp"
#include "Frame.h"
#include "Film.hpp"
Act::Act(const act_properties& _prop) :properties(_prop) {

RenderPass::Renderpass_Properties rp_prop;
    rp_prop.color_attachments = _prop.color_attachments;
    RenderPass::subpass subpass;
    subpass.bind_point = PipelineBindPoint::GRAPHICS;
    subpass.color_attachments.resize(_prop.color_attachments.size());
    for (int i = 0; i < _prop.color_attachments.size(); i++)
    {
        subpass.color_attachments[i].attachment = i;
        subpass.color_attachments[i].ideal_layout = ImageLayouts::COLOR_ATTACHMENT;
    }
    rp_prop.subpasses = {subpass};
    rp.emplace(rp_prop);

}

Act::~Act()
{
     if(rp.has_value()) rp->Destroy();

}


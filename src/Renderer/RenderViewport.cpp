#include "RenderViewport.hpp"
#include <VK/Shaders/Shader.hpp>
RenderViewport::RenderViewport(const RenderViewportCreateInfo &info)
{
    size = info.initial_size;
    RenderPass::attachment attach;
    attach.format = info.image_format;

    act_properties actp;
    actp.color_attachments.push_back(attach);

    act = std::make_shared<Act>(actp);

    frame_properties frmp;
    frmp.dimensions = uvec3(size,1);
    frmp.lens_subpass_id = 0;
    frame = std::make_shared<Frame>(frmp,*act);
}

void RenderViewport::SetSize(uvec2 new_size)
{
    size = new_size;
}


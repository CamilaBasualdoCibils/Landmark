#pragma once

#include <pch.h>
#include "Attachment.hpp"
#include <Graphics/Vulkan/Commands/CommandManager/VKCommandManager.hpp>
namespace Render
{
namespace Composite
{
struct PipelinePayload
{
    GPURef<VK::CommandManager> cmdManager;
};
struct Payload
{
    PipelinePayload pipelinePayload;
    GPURef<VK::RenderTarget> RenderTarget;
    boost::container::static_vector<LoadedColorAttachment,MAX_COLOR_ATTACHMENTS> ColorAttachments;
    std::optional<LoadedDepthStencilAttachment> DepthAttachment;
};
} // namespace Composite
} // namespace Render

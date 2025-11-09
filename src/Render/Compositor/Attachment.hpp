#pragma once
#include <Graphics/Texture.hpp>
#include <Graphics/Vulkan/Images/VKTexture.hpp>
#include <Graphics/Vulkan/Rendering/VKRenderTarget.hpp>
#include <Types/Flags.hpp>
#include <pch.h>
namespace Render
{
namespace Composite
{

struct ColorAttachmentInfo
{
    std::string Name = "NoName";
    std::optional<ivec2> DimensionsOverride; //is not set then inherits from parent.
    Flags<VK::ImageUsage> Usage = {VK::ImageUsage::eTransferDst};
    VK::Format format = VK::Format::eRGBA8_UNorm;
    VK::AttachmentLoadOp clearPolicy = VK::AttachmentLoadOp::eClear;
    std::optional<std::variant<vec4, uvec4, ivec4>> ClearValueColor = vec4(0.0f);
    // Color equivalent
};
const static inline uint32 MAX_COLOR_ATTACHMENTS = 8;
struct DepthStencilAttachmentInfo
{
    std::string Name = "NoName";
    std::optional<ivec2> DimensionsOverride; //is not set then inherits from parent.
    Flags<VK::ImageUsage> Usage = {VK::ImageUsage::eTransferDst};
    VK::Format format = VK::Format::eDepth32_SFloat;
    VK::AttachmentLoadOp clearPolicy = VK::AttachmentLoadOp::eClear;
    std::optional<double> ClearValueDepth = 1.0;
    std::optional<uint32> ClearValueStencil = 0;
};
struct LoadedAttachment
{
    GPURef<VK::Texture> Texture;
};
struct LoadedColorAttachment : LoadedAttachment
{
    ColorAttachmentInfo info;
};
struct LoadedDepthStencilAttachment : LoadedAttachment
{
    DepthStencilAttachmentInfo info;
};

} // namespace Composite

} // namespace Render

#pragma once
#include <pch.h>
#include <Graphics/Texture.hpp>
/// @brief Represents a singular Texture/Image for rendering
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
 struct CompositeAttachment
{
    Graphics::TextureFormat format = Graphics::TextureFormat::Values::eRGBA8_UNorm;
};   
} // namespace Render


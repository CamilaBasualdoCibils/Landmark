#pragma once
#include "Graphics/Compositor/ICompositeLayer.hpp"
#include <pch.h>
namespace Render
{
const static inline std::string AlbedoAttachmentName = "Albedo", NormalAttachmentName = "Normal",
                                PositionAttachmentName = "Position", MaterialAttachmentName = "Material",
                                DepthAttachmentName = "Depth",ColorAttachmentName = "Color";

const static inline Graphics::CompositeLayerAttachments GBufferStageAttachments = {
    {AlbedoAttachmentName,
     Graphics::CompositeLayerAttachmentProperties{.format = Graphics::TextureFormatValues::eRGB8_UNorm}},
    {NormalAttachmentName,
     Graphics::CompositeLayerAttachmentProperties{.format = Graphics::TextureFormatValues::eRGB16_SFloat}},
    {PositionAttachmentName,
     Graphics::CompositeLayerAttachmentProperties{.format = Graphics::TextureFormatValues::eRGB16_SFloat}},
    {MaterialAttachmentName,
     Graphics::CompositeLayerAttachmentProperties{.format = Graphics::TextureFormatValues::eRGBA8_UNorm}},
    {DepthAttachmentName,
     Graphics::CompositeLayerAttachmentProperties{.format = Graphics::TextureFormatValues::eDepth32_SFloat}}};

     const static inline Graphics::CompositeLayerAttachments PresentStageAttachments = {
    {ColorAttachmentName,
     Graphics::CompositeLayerAttachmentProperties{.format = Graphics::TextureFormatValues::eRGBA8_UNorm}},
    {DepthAttachmentName,
     Graphics::CompositeLayerAttachmentProperties{.format = Graphics::TextureFormatValues::eDepth32_SFloat}}};
} // namespace Render

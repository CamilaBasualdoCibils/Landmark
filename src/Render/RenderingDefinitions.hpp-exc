#pragma once
#include "Graphics/Compositor/ICompositeLayer.hpp"
#include <pch.h>
namespace Render
{
const static inline std::string AlbedoAttachmentName = "Albedo", NormalAttachmentName = "Normal",
                                PositionAttachmentName = "Position", MaterialAttachmentName = "Material",
                                DepthAttachmentName = "Depth", ColorAttachmentName = "Color";

const static inline Graphics::CompositeLayerAttachmentProperties AlbedoProperties{
    .format = Graphics::TextureFormat::Values::eRGBA8_UNorm,
    .AspectMask = VK::ImageAspect::eColor,
    .Usage = {VK::ImageUsage::eColorAttachment, VK::ImageUsage::eSampled, VK::ImageUsage::eTransferSrc,
              VK::ImageUsage::eTransferDst}};

const static inline Graphics::CompositeLayerAttachmentProperties NormalProperties{
    .format = Graphics::TextureFormat::Values::eRGBA16_SFloat,
    .AspectMask = VK::ImageAspect::eColor,
    .Usage = {VK::ImageUsage::eColorAttachment, VK::ImageUsage::eSampled, VK::ImageUsage::eTransferSrc,
              VK::ImageUsage::eTransferDst}};

const static inline Graphics::CompositeLayerAttachmentProperties PositionProperties{
    .format = Graphics::TextureFormat::Values::eRGBA16_SFloat,
    .AspectMask = VK::ImageAspect::eColor,
    .Usage = {VK::ImageUsage::eColorAttachment, VK::ImageUsage::eSampled, VK::ImageUsage::eTransferSrc,
              VK::ImageUsage::eTransferDst}};

const static inline Graphics::CompositeLayerAttachmentProperties MaterialProperties{
    .format = Graphics::TextureFormat::Values::eRGBA8_UNorm,
    .AspectMask = VK::ImageAspect::eColor,
    .Usage = {VK::ImageUsage::eColorAttachment, VK::ImageUsage::eSampled, VK::ImageUsage::eTransferSrc,
              VK::ImageUsage::eTransferDst}};

const static inline Graphics::CompositeLayerAttachmentProperties DepthProperties{
    .format = Graphics::TextureFormat::Values::eDepth32_SFloat,
    .AspectMask = VK::ImageAspect::eDepth,
    .Usage = {VK::ImageUsage::eDepthStencilAttachment, VK::ImageUsage::eSampled, VK::ImageUsage::eTransferSrc,
              VK::ImageUsage::eTransferDst}};

const static inline Graphics::CompositeLayerAttachmentProperties ColorProperties{
    .format = Graphics::TextureFormat::Values::eRGBA8_UNorm,
    .AspectMask = VK::ImageAspect::eColor,
    .Usage = {VK::ImageUsage::eColorAttachment, VK::ImageUsage::eSampled, VK::ImageUsage::eTransferSrc,
              VK::ImageUsage::eTransferDst}};

const static inline Graphics::CompositeLayerAttachments GBufferStageAttachments = {
    {AlbedoAttachmentName, AlbedoProperties},
    {NormalAttachmentName, NormalProperties},
    {PositionAttachmentName, PositionProperties},
    {MaterialAttachmentName, MaterialProperties},
    {DepthAttachmentName, DepthProperties}};

const static inline Graphics::CompositeLayerAttachments PresentStageAttachments = {
    {ColorAttachmentName, ColorProperties}, {DepthAttachmentName, DepthProperties}};
} // namespace Render

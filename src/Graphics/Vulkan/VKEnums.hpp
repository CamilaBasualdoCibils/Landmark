#pragma once
#include <pch.h>

namespace VK
{
enum class CompareOp : std::underlying_type_t<vk::CompareOp>
{
    eAlways = (size_t)vk::CompareOp::eAlways,
    eEqual = (size_t)vk::CompareOp::eEqual,
    eGreater = (size_t)vk::CompareOp::eGreater,
    eGreaterOrEqual = (size_t)vk::CompareOp::eGreaterOrEqual,
    eLess = (size_t)vk::CompareOp::eLess,
    eLessOrEqual = (size_t)vk::CompareOp::eLessOrEqual,
    eNever = (size_t)vk::CompareOp::eNever,
    eNotEqual = (size_t)vk::CompareOp::eNotEqual,
};
enum class CullMode : std::underlying_type_t<vk::CullModeFlagBits>
{
    eFront = (size_t)vk::CullModeFlagBits::eFront,
    eFrontAndBack = (size_t)vk::CullModeFlagBits::eFrontAndBack,
    eBack = (size_t)vk::CullModeFlagBits::eBack,
    eNone = (size_t)vk::CullModeFlagBits::eNone,
};
enum class ShaderStages : std::underlying_type_t<vk::ShaderStageFlagBits>
{
    eVertex = (size_t)vk::ShaderStageFlagBits::eVertex,
    eGeometry = (size_t)vk::ShaderStageFlagBits::eGeometry,
    eFragment = (size_t)vk::ShaderStageFlagBits::eFragment,
    eCompute = (size_t)vk::ShaderStageFlagBits::eCompute,
};
enum class IndexType : std::underlying_type_t<vk::IndexType>
{
    eUint8 = (size_t)vk::IndexType::eUint8,
    eUint16 = (size_t)vk::IndexType::eUint16,
    eUint32 = (size_t)vk::IndexType::eUint32,
};
enum class SamplerMipMapMode : std::underlying_type_t<vk::SamplerMipmapMode>
{
    eNearest = (size_t)vk::SamplerMipmapMode::eNearest,
    eLinear = (size_t)vk::SamplerMipmapMode::eLinear,
};
enum class SamplerAddressMode : std::underlying_type_t<vk::SamplerAddressMode>
{
    eClampToBorder = (size_t)vk::SamplerAddressMode::eClampToBorder,
    eClampToEdge = (size_t)vk::SamplerAddressMode::eClampToEdge,
    eMirrorClampToEdge = (size_t)vk::SamplerAddressMode::eMirrorClampToEdge,
    eMirroredRepeat = (size_t)vk::SamplerAddressMode::eMirroredRepeat,
    eRepeat = (size_t)vk::SamplerAddressMode::eRepeat,
};
enum class BufferUsage : std::underlying_type_t<vk::BufferUsageFlagBits>
{
    eIndexBuffer = (size_t)vk::BufferUsageFlagBits::eIndexBuffer,
    eIndirectBuffer = (size_t)vk::BufferUsageFlagBits::eIndirectBuffer,
    eTransferDst = (size_t)vk::BufferUsageFlagBits::eTransferDst,
    eTransferSrc = (size_t)vk::BufferUsageFlagBits::eTransferSrc,
    eUniformBuffer = (size_t)vk::BufferUsageFlagBits::eUniformBuffer,
    eVertexBuffer = (size_t)vk::BufferUsageFlagBits::eVertexBuffer,
};
enum class ImageUsage : std::underlying_type_t<vk::ImageUsageFlagBits>
{
    eSampled = (size_t)vk::ImageUsageFlagBits::eSampled,
    eColorAttachment = (size_t)vk::ImageUsageFlagBits::eColorAttachment,
    eDepthStencilAttachment = (size_t)vk::ImageUsageFlagBits::eDepthStencilAttachment,
    eTransferDst = (size_t)vk::ImageUsageFlagBits::eTransferDst,
    eTransferSrc = (size_t)vk::ImageUsageFlagBits::eTransferSrc,
};

enum MemoryProperties : std::underlying_type<vk::MemoryPropertyFlagBits>::type
{
    eDeviceLocal = (int)vk::MemoryPropertyFlagBits::eDeviceLocal,
    eHostCoherent = (int)vk::MemoryPropertyFlagBits::eHostCoherent,
    eHostVisible = (int)vk::MemoryPropertyFlagBits::eHostVisible,
};
enum class VertexInputRate : std::underlying_type_t<vk::VertexInputRate>
{
    eVertex = (size_t)vk::VertexInputRate::eVertex,
    eInstance = (size_t)vk::VertexInputRate::eInstance,
};
enum class ColorComponent : std::underlying_type_t<vk::ColorComponentFlagBits>
{
    eR = (size_t)vk::ColorComponentFlagBits::eR,
    eG = (size_t)vk::ColorComponentFlagBits::eG,
    eB = (size_t)vk::ColorComponentFlagBits::eB,
    eA = (size_t)vk::ColorComponentFlagBits::eA,
};
enum class BlendFactor : std::underlying_type_t<vk::BlendFactor>
{
    eSrcAlpha = (size_t)vk::BlendFactor::eSrcAlpha,
    eOneMinusSrcAlpha = (size_t)vk::BlendFactor::eOneMinusSrcAlpha,
    eOne = (size_t)vk::BlendFactor::eOne,
    eZero = (size_t)vk::BlendFactor::eZero,
};
enum class BlendOp : std::underlying_type_t<vk::BlendOp>
{
    eAdd = (size_t)vk::BlendOp::eAdd,
    eMax = (size_t)vk::BlendOp::eMax,
    eMin = (size_t)vk::BlendOp::eMin,
};
enum class ImageAspect : std::underlying_type_t<vk::ImageAspectFlagBits>
{
    eColor = (size_t)vk::ImageAspectFlagBits::eColor,
    eDepth = (size_t)vk::ImageAspectFlagBits::eDepth,
};
enum class Filter : std::underlying_type_t<vk::Filter>
{
    eNearest = (size_t)vk::Filter::eNearest,
    eLinear = (size_t)vk::Filter::eLinear
};
enum class Format : std::underlying_type<vk::Format>::type
{
    eRGBA8_UInt = (size_t)vk::Format::eR8G8B8A8Uint,
    eRGBA8_UNorm = (size_t)vk::Format::eR8G8B8A8Unorm,
    eRGB8_UNorm = (size_t)vk::Format::eR8G8B8Unorm, // uint8 RGB normalized
    eBGRA8_UNorm = (size_t)vk::Format::eB8G8R8A8Unorm,
    eBGRA8_SRGB = (size_t)vk::Format::eB8G8R8A8Srgb,
    eRGBA8_SRGB = (size_t)vk::Format::eR8G8B8A8Srgb,
    eRG32_SFloat = (size_t)vk::Format::eR32G32Sfloat,
    eRGB32_SFloat = (size_t)vk::Format::eR32G32B32Sfloat, // RGB 32bit signed float
    eRGB16_SFloat = (size_t)vk::Format::eR16G16B16Sfloat, // RGB 16bit signed float

    eRGBA32_SFloat = (size_t)vk::Format::eR32G32B32A32Sfloat,
    eDepth32_SFloat = (size_t)vk::Format::eD32Sfloat,
};
enum class ImageLayouts : std::underlying_type<vk::ImageLayout>::type
{
    eUndefined = (size_t)vk::ImageLayout::eUndefined,
    eColorAttachment = (size_t)vk::ImageLayout::eColorAttachmentOptimal,
    eDepthAttachment = (size_t)vk::ImageLayout::eDepthAttachmentOptimal,
    ePresentSrc = (size_t)vk::ImageLayout::ePresentSrcKHR,
    eTransferDst = (size_t)vk::ImageLayout::eTransferDstOptimal,
    eTransferSrc = (size_t)vk::ImageLayout::eTransferSrcOptimal,
    eShaderReadOnly = (size_t)vk::ImageLayout::eShaderReadOnlyOptimal,
    eGeneral = (size_t)vk::ImageLayout::eGeneral,
};
enum class DependencyFlags : std::underlying_type_t<vk::DependencyFlagBits>
{
};
enum class PipelineStageFlags : std::underlying_type_t<vk::PipelineStageFlagBits>
{
    eAllGraphics = (size_t)vk::PipelineStageFlagBits::eAllGraphics,
    eVertexShader = (size_t)vk::PipelineStageFlagBits::eVertexShader,
    eFragmentShader = (size_t)vk::PipelineStageFlagBits::eFragmentShader,
    eTopOfPipe = (size_t)vk::PipelineStageFlagBits::eTopOfPipe,
    eBottomOfPipe = (size_t)vk::PipelineStageFlagBits::eBottomOfPipe,
    eColorAttachmentOutput = (size_t)vk::PipelineStageFlagBits::eColorAttachmentOutput,
    eAllCommands = (size_t)vk::PipelineStageFlagBits::eAllCommands,
    eTransfer = (size_t)vk::PipelineStageFlagBits::eTransfer,
};
enum class PipelineBindPoint : std::underlying_type_t<vk::PipelineBindPoint>
{
    eGraphics = (size_t)vk::PipelineBindPoint::eGraphics,
    eCompute = (size_t)vk::PipelineBindPoint::eCompute,
    eRayTracing = (size_t)vk::PipelineBindPoint::eRayTracingKHR,
};
} // namespace VK

#pragma once

#include "Graphics/Texture.hpp"
#include "Graphics/Vulkan/Commands/CommandManager/VKCommandManager.hpp"
#include "Graphics/Vulkan/VKEnums.hpp"
#include "Types/Flags.hpp"
#include "misc/Conversions.hpp"
#include <pch.h>

namespace VK
{
// Copy Bitwise Image to image
struct CopyImageCommand : Command
{
    const std::variant<GPURef<Graphics::Texture>, vk::Image> SrcImage, DstImage;
    const ImageLayouts SrcImageLayout, DstImageLayout;
    const ivec3 SrcOffset, DstOffset, Extent;
    CopyImageCommand(std::variant<GPURef<Graphics::Texture>, vk::Image> srcImage,
                     std::variant<GPURef<Graphics::Texture>, vk::Image> dstImage, ImageLayouts srcImageLayout,
                     ImageLayouts dstImageLayout, ivec3 srcOffset, ivec3 dstOffset, ivec3 extent)
        : SrcImage(srcImage), DstImage(dstImage), SrcImageLayout(srcImageLayout), DstImageLayout(dstImageLayout),
          SrcOffset(srcOffset), DstOffset(dstOffset), Extent(glm::max(ivec3(1), extent))
    {
    }

    void Execute(GPURef<VK::CommandBuffer> cmdBuffer) override
    {
        vk::ImageCopy CopyRegion;
        CopyRegion.srcOffset = GlmToVkOffset(SrcOffset);
        CopyRegion.dstOffset = GlmToVkOffset(DstOffset);
        CopyRegion.extent = GlmToVkExtent((uvec3)Extent);
        CopyRegion.srcSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;

        CopyRegion.srcSubresource.mipLevel = 0;
        CopyRegion.srcSubresource.baseArrayLayer = 0;
        CopyRegion.srcSubresource.layerCount = 1;

        CopyRegion.dstSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
        CopyRegion.dstSubresource.mipLevel = 0;
        CopyRegion.dstSubresource.baseArrayLayer = 0;
        CopyRegion.dstSubresource.layerCount = 1;

        cmdBuffer->GetHandle().copyImage(
            std::holds_alternative<GPURef<Graphics::Texture>>(SrcImage)
                ? std::get<GPURef<Graphics::Texture>>(SrcImage)->VK().GetImage()->GetHandle()
                : std::get<vk::Image>(SrcImage),
            (vk::ImageLayout)SrcImageLayout,
            std::holds_alternative<GPURef<Graphics::Texture>>(DstImage)
                ? std::get<GPURef<Graphics::Texture>>(DstImage)->VK().GetImage()->GetHandle()
                : std::get<vk::Image>(DstImage),
            (vk::ImageLayout)DstImageLayout, {CopyRegion});
    }
    std::string GetName() override
    {
        return "CopyImage";
    }
};
// Copy ComponentWise Image to image. Preserves channels
struct BlitImageCommand : Command
{
    const std::variant<GPURef<Graphics::Texture>, vk::Image> SrcImage, DstImage;
    const ImageLayouts SrcImageLayout, DstImageLayout;
    const ivec3 SrcOffset, DstOffset, SrcExtent, DstExtent;
    const Filter Imagefilter;

    BlitImageCommand(std::variant<GPURef<Graphics::Texture>, vk::Image> srcImage,
                     std::variant<GPURef<Graphics::Texture>, vk::Image> dstImage, ImageLayouts srcImageLayout,
                     ImageLayouts dstImageLayout, VK::Filter imageFilter, ivec3 srcOffset, ivec3 dstOffset,
                     ivec3 srcExtent, std::optional<ivec3> dstExtent = std::nullopt)
        : SrcImage(srcImage), DstImage(dstImage), SrcImageLayout(srcImageLayout), DstImageLayout(dstImageLayout),
          SrcOffset(srcOffset), DstOffset(dstOffset), SrcExtent(glm::max(ivec3(1), srcExtent)),
          DstExtent(glm::max(ivec3(1), dstExtent.value_or(srcExtent))), Imagefilter(imageFilter)
    {
    }

    void Execute(GPURef<VK::CommandBuffer> cmdBuffer) override
    {
        vk::ImageBlit blitRegion{};
        blitRegion.srcSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
        blitRegion.srcSubresource.mipLevel = 0;
        blitRegion.srcSubresource.baseArrayLayer = 0;
        blitRegion.srcSubresource.layerCount = 1;
        blitRegion.srcOffsets[0] = GlmToVkOffset(SrcOffset);
        blitRegion.srcOffsets[1] = GlmToVkOffset(SrcOffset + SrcExtent);

        blitRegion.dstSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
        blitRegion.dstSubresource.mipLevel = 0;
        blitRegion.dstSubresource.baseArrayLayer = 0;
        blitRegion.dstSubresource.layerCount = 1;
        blitRegion.dstOffsets[0] = GlmToVkOffset(DstOffset);
        blitRegion.dstOffsets[1] = GlmToVkOffset(DstOffset + DstExtent);

        vk::Image srcImage = std::holds_alternative<GPURef<Graphics::Texture>>(SrcImage)
                                 ? std::get<GPURef<Graphics::Texture>>(SrcImage)->VK().GetImage()->GetHandle()
                                 : std::get<vk::Image>(SrcImage);
        vk::Image dstImage = std::holds_alternative<GPURef<Graphics::Texture>>(DstImage)
                                 ? std::get<GPURef<Graphics::Texture>>(DstImage)->VK().GetImage()->GetHandle()
                                 : std::get<vk::Image>(DstImage);

        cmdBuffer->GetHandle().blitImage(srcImage, static_cast<vk::ImageLayout>(SrcImageLayout), dstImage,
                                         static_cast<vk::ImageLayout>(DstImageLayout), 1, &blitRegion,
                                         (vk::Filter)Imagefilter);
    }

    std::string GetName() override
    {
        return "BlitImage";
    }
};

struct ClearColorImageCommand : Command
{
    const std::variant<GPURef<Graphics::Texture>, vk::Image> TargetImage;
    const ImageLayouts TargetImageLayout;
    const vec4 ClearColor;
    ClearColorImageCommand(std::variant<GPURef<Graphics::Texture>, vk::Image> TargetImage,
                           ImageLayouts TargetImageLayout, vec4 ClearColor)
        : TargetImage(TargetImage), TargetImageLayout(TargetImageLayout), ClearColor(ClearColor)
    {
    }

    void Execute(GPURef<VK::CommandBuffer> cmdBuffer) override
    {
        vk::ImageSubresourceRange subresourceRange = {vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1};
        vk::Image image = std::holds_alternative<GPURef<Graphics::Texture>>(TargetImage)
                              ? std::get<GPURef<Graphics::Texture>>(TargetImage)->VK().GetImage()->GetHandle()
                              : std::get<vk::Image>(TargetImage);
        vk::ClearColorValue clrV;
        clrV.setFloat32({ClearColor.r, ClearColor.g, ClearColor.b, ClearColor.a});
        cmdBuffer->GetHandle().clearColorImage(image, (vk::ImageLayout)TargetImageLayout, clrV, {subresourceRange});
    }
    std::string GetName() override
    {
        return "CopyImage";
    }
};
struct ClearDepthStencilImageCommand : Command
{
    const std::variant<GPURef<Graphics::Texture>, vk::Image> TargetImage;
    const ImageLayouts TargetImageLayout;
    const std::optional<float> DepthClearValue;
    const std::optional<uint32_t> StencilClearValue = 0;
    ClearDepthStencilImageCommand(
        std::variant<GPURef<Graphics::Texture>, vk::Image> TargetImage, ImageLayouts TargetImageLayout,
        std::optional<float> DepthClearValue = std::nullopt, std::optional<uint32_t> StencilClearValue = std::nullopt)
        : TargetImage(TargetImage), TargetImageLayout(TargetImageLayout), DepthClearValue(DepthClearValue),
          StencilClearValue(StencilClearValue)
    {
    }

    void Execute(GPURef<VK::CommandBuffer> cmdBuffer) override
    {
        vk::Image image = std::holds_alternative<GPURef<Graphics::Texture>>(TargetImage)
                              ? std::get<GPURef<Graphics::Texture>>(TargetImage)->VK().GetImage()->GetHandle()
                              : std::get<vk::Image>(TargetImage);

        vk::ClearDepthStencilValue value;
        value.stencil = StencilClearValue.value_or(0);
        value.depth = DepthClearValue.value_or(1.0f);
        vk::ImageSubresourceRange subresourceRange = {vk::ImageAspectFlagBits::eNone, 0, 1, 0, 1};
        if (DepthClearValue.has_value())
            subresourceRange.aspectMask |= vk::ImageAspectFlagBits::eDepth;
        if (StencilClearValue.has_value())
            subresourceRange.aspectMask |= vk::ImageAspectFlagBits::eStencil;
        cmdBuffer->GetHandle().clearDepthStencilImage(image, (vk::ImageLayout)TargetImageLayout, value, {subresourceRange});
    }
    std::string GetName() override
    {
        return "CopyImage";
    }
};
} // namespace VK
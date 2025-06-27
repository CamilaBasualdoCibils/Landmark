#pragma once

#include "Graphics/Texture.hpp"
#include "Graphics/Vulkan/CommandBuffer/CommandBuffer.hpp"
#include "Graphics/Vulkan/Images/ImageEnums.hpp"
#include "Types/Flags.h"
#include "misc/Conversions.hpp"
#include <pch.h>
#include <vulkan/vulkan_structs.hpp>

namespace VK
{
struct CopyImage : Command
{
    const std::variant<GPURef<Graphics::Texture>, vk::Image> SrcImage, DstImage;
    const ImageLayouts SrcImageLayout, DstImageLayout;
    const ivec3 SrcOffset, DstOffset, Extent;
    CopyImage(std::variant<GPURef<Graphics::Texture>, vk::Image> srcImage,
              std::variant<GPURef<Graphics::Texture>, vk::Image> dstImage, ImageLayouts srcImageLayout,
              ImageLayouts dstImageLayout, ivec3 srcOffset, ivec3 dstOffset, ivec3 extent)
        : SrcImage(srcImage), DstImage(dstImage), SrcImageLayout(srcImageLayout), DstImageLayout(dstImageLayout),
          SrcOffset(srcOffset), DstOffset(dstOffset), Extent(glm::max(ivec3(1), extent))
    {
    }

    void Execute(void *context) override
    {
        vk::ImageCopy CopyRegion;
        CopyRegion.srcOffset = GlmToVk(SrcOffset);
        CopyRegion.dstOffset = GlmToVk(DstOffset);
        CopyRegion.extent = GlmToVk((uvec3)Extent);
        CopyRegion.srcSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;

        CopyRegion.srcSubresource.mipLevel = 0;
        CopyRegion.srcSubresource.baseArrayLayer = 0;
        CopyRegion.srcSubresource.layerCount = 1;

        CopyRegion.dstSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
        CopyRegion.dstSubresource.mipLevel = 0;
        CopyRegion.dstSubresource.baseArrayLayer = 0;
        CopyRegion.dstSubresource.layerCount = 1;
        vk::CommandBuffer *cmdBuffer = (vk::CommandBuffer *)context;

        cmdBuffer->copyImage(std::holds_alternative<GPURef<Graphics::Texture>>(SrcImage)
                                 ? std::get<GPURef<Graphics::Texture>>(SrcImage)->VK().GetImage().GetHandle()
                                 : std::get<vk::Image>(SrcImage),
                             (vk::ImageLayout)SrcImageLayout,
                             std::holds_alternative<GPURef<Graphics::Texture>>(DstImage)
                                 ? std::get<GPURef<Graphics::Texture>>(DstImage)->VK().GetImage().GetHandle()
                                 : std::get<vk::Image>(DstImage),
                             (vk::ImageLayout)DstImageLayout, {CopyRegion});
    }
    std::string GetName() override
    {
        return "CopyImage";
    }
};

struct ClearColorImage : Command
{
    const std::variant<GPURef<Graphics::Texture>, vk::Image> TargetImage;
    const ImageLayouts TargetImageLayout;
    const vec4 ClearColor;
    ClearColorImage(std::variant<GPURef<Graphics::Texture>, vk::Image> TargetImage, ImageLayouts TargetImageLayout,
                    vec4 ClearColor)
        : TargetImage(TargetImage), TargetImageLayout(TargetImageLayout), ClearColor(ClearColor)
    {
    }

    void Execute(void *context) override
    {
        vk::CommandBuffer *cmdBuffer = (vk::CommandBuffer *)context;
        vk::ImageSubresourceRange subresourceRange = {vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1};
        vk::Image image = std::holds_alternative<GPURef<Graphics::Texture>>(TargetImage)
                              ? std::get<GPURef<Graphics::Texture>>(TargetImage)->VK().GetImage().GetHandle()
                              : std::get<vk::Image>(TargetImage);
        vk::ClearColorValue clrV;
        clrV.setFloat32({ClearColor.r, ClearColor.g, ClearColor.b, ClearColor.a});

        cmdBuffer->clearColorImage(image, (vk::ImageLayout)TargetImageLayout, clrV, {subresourceRange});
    }
    std::string GetName() override
    {
        return "CopyImage";
    }
};
} // namespace VK
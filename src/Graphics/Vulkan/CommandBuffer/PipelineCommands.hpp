#pragma once
#include "Graphics/Texture.hpp"
#include "Graphics/Vulkan/CommandBuffer/CommandBuffer.hpp"
#include "Graphics/Vulkan/Images/ImageEnums.hpp"
#include "Graphics/Vulkan/Pipeline/PipelineEnums.hpp"
#include "Types/Flags.h"
#include <pch.h>
#include <vulkan/vulkan_enums.hpp>
#include <vulkan/vulkan_handles.hpp>
#include <vulkan/vulkan_structs.hpp>

namespace VK
{
struct TransitionImageLayout : Command
{
    std::variant<GPURef<Graphics::Texture>, vk::Image> Image;
    vk::ImageMemoryBarrier barrier;
    ImageLayouts OldLayout, NewLayout;
    Flags<PipelineStageFlags> SrcStage, DstStage;
    TransitionImageLayout(std::variant<GPURef<Graphics::Texture>, vk::Image> image, ImageLayouts oldLayout,
                          ImageLayouts newLayout, Flags<PipelineStageFlags> srcStage,
                          Flags<PipelineStageFlags> dstStage)
    {
        Image = image;
        OldLayout = oldLayout;
        NewLayout = newLayout;
        SrcStage = srcStage;
        DstStage = dstStage;
        OldLayout = oldLayout;
        vk::ImageSubresourceRange range = {vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1};

        barrier.oldLayout = (vk::ImageLayout)OldLayout;
        barrier.newLayout = (vk::ImageLayout)NewLayout;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        if (std::holds_alternative<GPURef<Graphics::Texture>>(Image))
            barrier.image = std::get < GPURef<Graphics::Texture>>(Image)->VK().GetImage();
        else if (std::holds_alternative<vk::Image>(Image))
        {
            barrier.image = std::get<vk::Image>(image);
        }
        else
            LASSERT(false, "Invalid Use");
        barrier.subresourceRange = range;

        switch (oldLayout)
        {
        case ImageLayouts::eUndefined:
            barrier.srcAccessMask = vk::AccessFlagBits::eNone;
            break;
        case ImageLayouts::eColorAttachment:
            barrier.srcAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
            break;
        case ImageLayouts::eGeneral:
            barrier.srcAccessMask = vk::AccessFlagBits::eShaderWrite | vk::AccessFlagBits::eShaderRead;
            break;
        // Add more as needed
        default:
            barrier.srcAccessMask = vk::AccessFlagBits::eNone;
            break;
        }

        switch (newLayout)
        {
        case ImageLayouts::eColorAttachment:
            barrier.dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
            break;
        case ImageLayouts::ePresentSrc:
            barrier.dstAccessMask = vk::AccessFlagBits::eNone;
            break;
        case ImageLayouts::eGeneral:
            barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead | vk::AccessFlagBits::eShaderWrite;
            break;
        // Add more as needed
        default:
            barrier.dstAccessMask = vk::AccessFlagBits::eNone;
            break;
        }
    }

    std::string GetName() override {return "TransitionImageLayout";}
    void Execute(void *context) override
    {
        vk::CommandBuffer *cmdBuffer = (vk::CommandBuffer *)context;
        cmdBuffer->pipelineBarrier(SrcStage, DstStage, (vk::DependencyFlags)0, {}, {}, {barrier});
    }
};
struct PipelineBarrier : Command
{
    Flags<VK::PipelineStageFlags> srcStageMask, dstStageMask;
    Flags<VK::DependencyFlags> dependency;
    PipelineBarrier(Flags<VK::PipelineStageFlags> srcStageMask, Flags<VK::PipelineStageFlags> dstStageMask,
                    Flags<VK::DependencyFlags> dependency);
    void Execute(void *context) override
    {
        vk::CommandBuffer *cmdBuffer = (vk::CommandBuffer *)context;
        cmdBuffer->pipelineBarrier(srcStageMask, dstStageMask, dependency, 0, 0, 0);
    }

  protected:
};
} // namespace VK
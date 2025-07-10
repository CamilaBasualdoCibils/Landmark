#pragma once
#include <pch.h>
#include "Graphics/Texture.hpp"
#include "Graphics/Vulkan/Commands/CommandManager/CommandManager.hpp"
#include "Graphics/Vulkan/Enums.hpp"
#include "Types/Flags.h"
#include "Graphics/Vulkan/Pipeline/Pipeline.hpp"


namespace VK
{
struct BindPipelineCommand : Command
{
    GPURef<VK::Pipeline> pipeline;
    vk::PipelineBindPoint BindPoint;
    BindPipelineCommand(GPURef<VK::Pipeline> pipeline, vk::PipelineBindPoint BindPoint)
        : pipeline(pipeline), BindPoint(BindPoint)
    {
    }

    std::string GetName() override
    {
        return "BindPipeline";
    }
    void Execute(GPURef<VK::CommandBuffer> cmdBuffer) override
    {
        cmdBuffer->GetHandle().bindPipeline(BindPoint,*pipeline);
    }
};
struct TransitionImageLayoutCommand : Command
{
    std::variant<GPURef<Graphics::Texture>, vk::Image> Image;
    vk::ImageMemoryBarrier barrier;
    ImageLayouts OldLayout, NewLayout;
    Flags<PipelineStageFlags> SrcStage, DstStage;
    TransitionImageLayoutCommand(std::variant<GPURef<Graphics::Texture>, vk::Image> image, ImageLayouts oldLayout,
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
            barrier.image = *std::get<GPURef<Graphics::Texture>>(Image)->VK().GetImage();
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

    std::string GetName() override
    {
        return "TransitionImageLayout";
    }
    void Execute(GPURef<VK::CommandBuffer> cmdBuffer) override
    {

        cmdBuffer->GetHandle().pipelineBarrier(SrcStage, DstStage, (vk::DependencyFlags)0, {}, {}, {barrier});
    }
};
struct PipelineBarrierCommand : Command
{
    Flags<VK::PipelineStageFlags> srcStageMask, dstStageMask;
    Flags<VK::DependencyFlags> dependency;
    PipelineBarrierCommand(Flags<VK::PipelineStageFlags> srcStageMask, Flags<VK::PipelineStageFlags> dstStageMask,
                    Flags<VK::DependencyFlags> dependency);
    void Execute(GPURef<VK::CommandBuffer> cmdBuffer) override
    {
        cmdBuffer->GetHandle().pipelineBarrier(srcStageMask, dstStageMask, dependency, 0, 0, 0);
    }

  protected:
};
} // namespace VK
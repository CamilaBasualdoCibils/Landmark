#include "VKPipelineLayout.hpp"
#include <Graphics/GraphicsEngine.hpp>

VK::PipelineLayout::PipelineLayout(const PipelineLayoutProperties &Properties)
{
    vk::PipelineLayoutCreateInfo CreateInfo;
    std::vector<vk::PushConstantRange> PushConstantRanges;
    for (const auto& Range : Properties.PushConstants)
    {
        vk::PushConstantRange VkRange;
        VkRange.offset = Range.Offset;
        VkRange.size = Range.Size;
        VkRange.stageFlags = Range.Stages;
    }
    CreateInfo.setPushConstantRanges(PushConstantRanges);
    const auto CreateResult = GraphicsEngine::Get().GetMainGPU()->VK()->LogicalDevice()->GetHandle().createPipelineLayout(CreateInfo);
    LASSERT(CreateResult.result == vk::Result::eSuccess,"oh no");
    Handle = CreateResult.value;
}

VK::PipelineLayout::~PipelineLayout()
{
    GraphicsEngine::Get().GetMainGPU()->VK()->LogicalDevice()->GetHandle().destroyPipelineLayout(Handle);
}

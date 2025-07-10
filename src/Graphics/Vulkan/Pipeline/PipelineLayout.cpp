#include "PipelineLayout.hpp"
#include <Graphics/GraphicsEngine.hpp>

VK::PipelineLayout::PipelineLayout(const PipelineLayoutProperties &Properties)
{
    vk::PipelineLayoutCreateInfo CreateInfo;
    const auto CreateResult = GraphicsEngine::Get().GetMainGPU()->VK()->LogicalDevice()->GetHandle().createPipelineLayout(CreateInfo);
    LASSERT(CreateResult.result == vk::Result::eSuccess,"oh no");
    Handle = CreateResult.value;
}

VK::PipelineLayout::~PipelineLayout()
{
    GraphicsEngine::Get().GetMainGPU()->VK()->LogicalDevice()->GetHandle().destroyPipelineLayout(Handle);
}

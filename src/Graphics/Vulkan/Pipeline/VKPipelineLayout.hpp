#pragma once
#include <pch.h>
#include "Graphics/Vulkan/VKEnums.hpp"
#include "Types/Flags.hpp"
namespace VK
{
struct PipelineLayoutPushConstantRange
{
uint32_t Offset = 0,Size = 0;
Flags<VK::ShaderStages> Stages;
};
struct PipelineLayoutProperties
{
    std::vector<PipelineLayoutPushConstantRange> PushConstants;
};
class PipelineLayout
{
    vk::PipelineLayout Handle;

  public:
    PipelineLayout(const PipelineLayoutProperties &Properties);
    ~PipelineLayout();
    operator vk::PipelineLayout() const
    {
        return Handle;
    }
};
} // namespace VK
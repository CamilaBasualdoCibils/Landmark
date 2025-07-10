#pragma once
#include <pch.h>

namespace VK
{

struct PipelineLayoutProperties
{
    
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
#pragma once

#include "Graphics/Vulkan/VKEnums.hpp"

#include <pch.h>

namespace Graphics
{
enum PipelineBindPoint : std::underlying_type_t<VK::PipelineBindPoint>
{
    eGraphics = (size_t)VK::PipelineBindPoint::eGraphics,
    eCompute = (size_t)VK::PipelineBindPoint::eCompute,
    eRayTracing = (size_t)VK::PipelineBindPoint::eRayTracing,
};
} // namespace Graphics
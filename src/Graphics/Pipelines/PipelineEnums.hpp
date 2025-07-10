#pragma once

#include "Graphics/Vulkan/Enums.hpp"

#include <pch.h>
#include <type_traits>

namespace Graphics
{
enum PipelineBindPoint : std::underlying_type_t<VK::PipelineBindPoint>
{
    eGraphics = (size_t)VK::PipelineBindPoint::eGraphics,
    eCompute = (size_t)VK::PipelineBindPoint::eCompute,
    eRayTracing = (size_t)VK::PipelineBindPoint::eRayTracing,
};
} // namespace Graphics
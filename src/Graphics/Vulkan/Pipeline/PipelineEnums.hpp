#pragma once
#include <cstddef>
#include <pch.h>
#include <type_traits>
#include <vulkan/vulkan_enums.hpp>

namespace VK
{
enum PipelineBindPoint : std::underlying_type_t<vk::PipelineBindPoint>
{
    eGraphics = (size_t)vk::PipelineBindPoint::eGraphics,
    eCompute = (size_t)vk::PipelineBindPoint::eCompute,
    eRayTracing = (size_t)vk::PipelineBindPoint::eRayTracingKHR,
};
} // namespace VK

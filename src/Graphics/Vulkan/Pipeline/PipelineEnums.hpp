#pragma once
#include <cstddef>
#include <pch.h>
#include <type_traits>
#include <vulkan/vulkan_enums.hpp>

namespace VK
{
enum class DependencyFlags : std::underlying_type_t<vk::DependencyFlagBits>
{
};
enum class PipelineStageFlags : std::underlying_type_t<vk::PipelineStageFlagBits>
{
    eAllGraphics = (size_t)vk::PipelineStageFlagBits::eAllGraphics,
    eVertexShader = (size_t)vk::PipelineStageFlagBits::eVertexShader,
    eFragmentShader = (size_t)vk::PipelineStageFlagBits::eFragmentShader,
    eTopOfPipe = (size_t)vk::PipelineStageFlagBits::eTopOfPipe,
    eBottomOfPipe = (size_t)vk::PipelineStageFlagBits::eBottomOfPipe,
    eColorAttachmentOutput = (size_t)vk::PipelineStageFlagBits::eColorAttachmentOutput,
    eAllCommands = (size_t)vk::PipelineStageFlagBits::eAllCommands,
    eTransfer = (size_t)vk::PipelineStageFlagBits::eTransfer,
};
enum class PipelineBindPoint : std::underlying_type_t<vk::PipelineBindPoint>
{
    eGraphics = (size_t)vk::PipelineBindPoint::eGraphics,
    eCompute = (size_t)vk::PipelineBindPoint::eCompute,
    eRayTracing = (size_t)vk::PipelineBindPoint::eRayTracingKHR,
};
} // namespace VK

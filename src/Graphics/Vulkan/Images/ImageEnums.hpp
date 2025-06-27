#pragma once
#include <pch.h>
#include <vulkan/vulkan_enums.hpp>

namespace VK
{
enum class ImageFormats : std::underlying_type<vk::Format>::type
{
    eRGBA8_UInt = (int)vk::Format::eR8G8B8A8Uint,
    eRGBA8_UNorm = (int)vk::Format::eR8G8B8A8Unorm
};
enum class ImageLayouts : std::underlying_type<vk::ImageLayout>::type
{
    eUndefined = (size_t)vk::ImageLayout::eUndefined,
    eColorAttachment = (size_t)vk::ImageLayout::eColorAttachmentOptimal,
    eDepthAttachment = (size_t)vk::ImageLayout::eDepthAttachmentOptimal,
    ePresentSrc = (size_t)vk::ImageLayout::ePresentSrcKHR,
    eTransferDst = (size_t)vk::ImageLayout::eTransferDstOptimal,
    eTransferSrc = (size_t)vk::ImageLayout::eTransferSrcOptimal,
    eShaderReadOnly = (size_t)vk::ImageLayout::eShaderReadOnlyOptimal,
};
} // namespace VK
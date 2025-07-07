#pragma once
#include <pch.h>
#include <type_traits>
#include <vulkan/vulkan_enums.hpp>

namespace VK
{
enum class Filter : std::underlying_type_t<vk::Filter>
{
    eNearest = (size_t)vk::Filter::eNearest,
    eLinear = (size_t)vk::Filter::eLinear
};
enum class ImageFormats : std::underlying_type<vk::Format>::type
{
    eRGBA8_UInt = (int)vk::Format::eR8G8B8A8Uint,
    eRGBA8_UNorm = (int)vk::Format::eR8G8B8A8Unorm,
    eBGRA8_UNorm = (int)vk::Format::eB8G8R8A8Unorm,
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
    eGeneral = (size_t)vk::ImageLayout::eGeneral,
};
} // namespace VK
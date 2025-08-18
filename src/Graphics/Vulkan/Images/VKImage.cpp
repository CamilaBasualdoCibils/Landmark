#include "VKImage.hpp"
#include "Graphics/GraphicsEngine.hpp"
#include "Graphics/Vulkan/Memory/VKMemory.hpp"
#include "misc/Conversions.hpp"
#include "pch.h"
VK::Image::Image(const VK::ImageProperties &Properties) : Properties(Properties)
{
    // vk::ExternalMemoryImageCreateInfo ExternalImageInfo;
    // ExternalImageInfo.handleTypes = vk::ExternalMemoryHandleTypeFlagBits::eOpaqueFd;
    vk::ImageCreateInfo CreateInfo;
    CreateInfo.format = (vk::Format)Properties.format;
    CreateInfo.arrayLayers = Properties.Layers;
    CreateInfo.extent = GlmToVkExtent(max(Properties.Dimensions, uvec3(1)));
    CreateInfo.mipLevels = Properties.MipLevels;
    CreateInfo.tiling = vk::ImageTiling::eOptimal;
    CreateInfo.initialLayout = vk::ImageLayout::eUndefined;
    CreateInfo.usage = Properties.Usage;
    CreateInfo.sharingMode = vk::SharingMode::eExclusive;
    CreateInfo.samples = vk::SampleCountFlagBits::e1;
    CreateInfo.imageType = Properties.Dimensions.y > 1
                               ? (Properties.Dimensions.z > 1 ? vk::ImageType::e3D : vk::ImageType::e2D)
                               : vk::ImageType::e1D;
    // CreateInfo.pNext = &ExternalImageInfo;
    const vk::Result CreateResult =
        GraphicsEngine::Get().GetMainGPU()->VK()->GetAllocator()->CreateImage(CreateInfo, handle, allocation);
    LASSERT(CreateResult == vk::Result::eSuccess, "Fucking hell");
}

VK::Image::~Image()
{
    GraphicsEngine::Get().GetMainGPU()->VK()->GetAllocator()->DestroyImage(handle, allocation);
}

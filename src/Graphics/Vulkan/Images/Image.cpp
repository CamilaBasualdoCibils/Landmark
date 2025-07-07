#include "Image.hpp"
#include "Graphics/GraphicsEngine.hpp"
#include "Graphics/Vulkan/Memory/Memory.hpp"
#include "misc/Conversions.hpp"
#include "pch.h"
#include <glm/fwd.hpp>
#include <glm/vector_relational.hpp>
#include <vulkan/vulkan_enums.hpp>
VK::Image::Image(const VK::ImageProperties &Properties)
{
    vk::ExternalMemoryImageCreateInfo ExternalImageInfo;
    ExternalImageInfo.handleTypes = vk::ExternalMemoryHandleTypeFlagBits::eOpaqueFd;
    vk::ImageCreateInfo CreateInfo;
    CreateInfo.format = (vk::Format)Properties.Format;
    CreateInfo.arrayLayers = Properties.Layers;
    CreateInfo.extent = GlmToVk(max(Properties.Dimensions, uvec3(1)));
    CreateInfo.mipLevels = Properties.MipLevels;
    CreateInfo.tiling = vk::ImageTiling::eOptimal;
    CreateInfo.initialLayout = vk::ImageLayout::eUndefined;
    CreateInfo.usage = vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled | vk::ImageUsageFlagBits::eTransferSrc;
    CreateInfo.sharingMode = vk::SharingMode::eExclusive;
    CreateInfo.samples = vk::SampleCountFlagBits::e1;
    CreateInfo.imageType = Properties.Dimensions.y > 1
                               ? (Properties.Dimensions.z > 1 ? vk::ImageType::e3D : vk::ImageType::e2D)
                               : vk::ImageType::e1D;
    CreateInfo.pNext = &ExternalImageInfo;
    const auto CreateResult =
        GraphicsEngine::Get().GetMainGPU()->VK()->LogicalDevice()->GetHandle().createImage(CreateInfo);
    LASSERT(CreateResult.result == vk::Result::eSuccess, "Fucking hell");
    handle = CreateResult.value;

    vk::MemoryRequirements MemRequirements =
        GraphicsEngine::Get().GetMainGPU()->VK()->LogicalDevice()->GetHandle().getImageMemoryRequirements(handle);

    std::optional<VK::MemoryType> MemType = GraphicsEngine::Get().GetMainGPU()->VK()->LogicalDevice()->FindMemoryFor(
        MemRequirements, Properties.memoryProperties);
    LASSERT(MemType.has_value(), "Dang");

    memory.emplace(*MemType, MemRequirements.size);
    const auto BindResult =
        GraphicsEngine::Get().GetMainGPU()->VK()->LogicalDevice()->GetHandle().bindImageMemory(handle, *memory, 0);
    LASSERT(BindResult == vk::Result::eSuccess, "shiet");
}
#pragma once
#include "Graphics/Interop/Interop.hpp"
#include "Graphics/Vulkan/Memory/VKMemory.hpp"
#include "Graphics/Vulkan/VKEnums.hpp"
#include "Types/Flags.hpp"
#include <pch.h>
#include "Graphics/Vulkan/Memory/VKAllocation.hpp"
namespace VK
{

struct ImageProperties
{
    uvec3 Dimensions;
    Format format;
    uint32_t Layers = 1;
    uint32_t MipLevels = 1;
    uint32_t Samples = 1; // only relevant for images used as render attachments
    Flags<MemoryProperties> memoryProperties = MemoryProperties::eDeviceLocal;
    Flags<ImageUsage> Usage = {ImageUsage::eTransferDst, ImageUsage::eSampled, ImageUsage::eTransferSrc};
};
class Image : public InteropGiver
{
    vk::Image handle;
    VK::Allocation allocation;
    const ImageProperties Properties;

  public:
    Image(const ImageProperties &Properties);
    ~Image();
    InteropTransaction ExportMemory() override
    {
        return allocation.ExportMemory();
    }
    vk::Image GetHandle() const
    {
        return handle;
    }
    operator vk::Image() const
    {
        return GetHandle();
    }

    const ImageProperties &GetProperties() const
    {
        return Properties;
    }
};
} // namespace VK
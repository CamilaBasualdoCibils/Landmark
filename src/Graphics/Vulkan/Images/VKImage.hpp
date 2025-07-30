#pragma once
#include "Graphics/Interop/Interop.hpp"
#include "Graphics/Vulkan/VKEnums.hpp"
#include "Graphics/Vulkan/Memory/VKMemory.hpp"
#include "Types/Flags.hpp"
#include <pch.h>

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
};
class Image : public InteropGiver
{
    vk::Image handle;
    std::optional<VK::Memory> memory;
    const ImageProperties Properties;

  public:
    Image(const ImageProperties &Properties);

    InteropTransaction ExportMemory() const
    {
        return memory->ExportMemory();
    }
    vk::Image GetHandle() const
    {
        return handle;
    }
    operator vk::Image() const
    {
        return GetHandle();
    }

    const ImageProperties& GetProperties() const {return Properties;}
};
} // namespace VK
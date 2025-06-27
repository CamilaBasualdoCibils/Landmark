#pragma once
#include "Graphics/Interop/Interop.hpp"
#include <pch.h>
#include <type_traits>
#include <vulkan/vulkan_enums.hpp>
#include <vulkan/vulkan_handles.hpp>
#include "Graphics/Vulkan/Memory/Memory.hpp"
#include "Types/Flags.h"
#include "ImageEnums.hpp"
namespace VK
{

struct ImageProperties
{
    uvec3 Dimensions;
    ImageFormats Format;
    uint32_t Layers = 1;
    uint32_t MipLevels = 1;
    uint32_t Samples = 1; // only relevant for images used as render attachments
    Flags<MemoryProperties> memoryProperties = MemoryProperties::eDeviceLocal;
};
class Image : public InteropGiver
{
    vk::Image handle;
    std::optional<VK::Memory> memory;

  public:
    Image(const ImageProperties& Properties);

    InteropTransaction ExportMemory() const {return memory->ExportMemory();}
    vk::Image GetHandle() const {return handle;}
    operator vk::Image() const {return GetHandle();}

};
} // namespace VK
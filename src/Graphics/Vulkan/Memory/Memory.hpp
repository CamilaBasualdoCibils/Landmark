#pragma once
#include "Graphics/Interop/Interop.hpp"
#include <pch.h>
#include <type_traits>
#include <vulkan/vulkan_enums.hpp>

namespace VK
{
    enum MemoryProperties : std::underlying_type<vk::MemoryPropertyFlagBits>::type
    {
        eDeviceLocal = (int)vk::MemoryPropertyFlagBits::eDeviceLocal,
        eHostCoherent = (int)vk::MemoryPropertyFlagBits::eHostCoherent,
        eHostVisible = (int)vk::MemoryPropertyFlagBits::eHostVisible,
    };
struct MemoryType {
    uint32_t Index;
    vk::MemoryPropertyFlags flags;
};
class Memory : public InteropGiver
{
    vk::DeviceMemory handle;
    FDHandle fdHandle;
    size_t Size;

  public:
    Memory(const MemoryType& Type,size_t size);

    InteropTransaction ExportMemory() const {return {fdHandle,Size};}

    operator vk::DeviceMemory() const {return handle;}
};
} // namespace VK
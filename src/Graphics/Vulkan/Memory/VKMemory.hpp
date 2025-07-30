#pragma once
#include "Graphics/Interop/Interop.hpp"
#include <pch.h>

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
    vk::DeviceMemory Handle;
    FDHandle fdHandle;
    size_t Size;
    void* MappedMemory = nullptr;

  public:
    Memory(const MemoryType& Type,size_t size);
    ~Memory();
    InteropTransaction ExportMemory() const {return {fdHandle,Size};}

    operator vk::DeviceMemory() const {return Handle;}
    void* Map();
    void Unmap();
};
} // namespace VK
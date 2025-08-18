#pragma once
#include "Graphics/Interop/Interop.hpp"
#include <pch.h>

namespace VK
{
    
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
    InteropTransaction ExportMemory() {return {fdHandle,Size};}

    operator vk::DeviceMemory() const {return Handle;}
    void* Map();
    void Unmap();
};
} // namespace VK
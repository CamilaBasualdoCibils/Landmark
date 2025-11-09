#pragma once
#include <Graphics/Interop/Interop.hpp>
#include <Graphics/Vulkan/Memory/VKMemory.hpp>
#include <Graphics/Vulkan/VKEnums.hpp>
#include <Types/Flags.hpp>
#include <pch.h>
#include "Graphics/Vulkan/Memory/VKAllocation.hpp"
namespace VK
{
struct BufferProperties
{
    size_t Size;
    Flags<BufferUsage> UsageFlags = {BufferUsage::eTransferSrc, BufferUsage::eTransferDst};
    Flags<MemoryProperties> memoryProperties = {MemoryProperties::eDeviceLocal, MemoryProperties::eHostCoherent,
                                                MemoryProperties::eHostVisible};
};
class Buffer : InteropGiver
{
    vk::Buffer Handle;
    VK::Allocation Allocation;

  public:
    Buffer(const BufferProperties &Properties);
    ~Buffer();
    InteropTransaction ExportMemory() override;
    operator vk::Buffer() const {return Handle;}
    void* Map() {return Allocation.Map();}
    void Unmap() { Allocation.UnMap();}
    void InsertData(void* data, size_t size, size_t offset);
};
} // namespace VK
#pragma once
#include <Graphics/Interop/Interop.hpp>
#include <Graphics/Vulkan/Memory/Memory.hpp>
#include <Graphics/Vulkan/Enums.hpp>
#include <Types/Flags.h>
#include <pch.h>
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
    GPURef<VK::Memory> Memory;

  public:
    Buffer(const BufferProperties &Properties);
    InteropTransaction ExportMemory() const override;
    operator vk::Buffer() const {return Handle;}
    void* Map() {return Memory->Map();}
    void Unmap() { Memory->Unmap();}
};
} // namespace VK
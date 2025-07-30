#pragma once
#include <Graphics/Interop/Interop.hpp>
#include <Graphics/Vulkan/Memory/VKMemory.hpp>
#include <Graphics/Vulkan/VKEnums.hpp>
#include <Types/Flags.hpp>
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
    ~Buffer();
    InteropTransaction ExportMemory() const override;
    operator vk::Buffer() const {return Handle;}
    void* Map() {return Memory->Map();}
    void Unmap() { Memory->Unmap();}
};
} // namespace VK
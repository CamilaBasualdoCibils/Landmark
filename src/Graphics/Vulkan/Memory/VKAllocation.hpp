#pragma once
#include "Graphics/GPURef.hpp"
#include "Graphics/Interop/Interop.hpp"
#include <pch.h>
namespace VK
{
class Device;
class Allocator;
struct Allocation : public InteropGiver
{
    VmaAllocation alloc;
    VmaAllocationInfo info;
    GPURef<Allocator> allocator;
    GPURef_weak<Device> device;

    std::optional<InteropTransaction> InteropTrans;

    operator VmaAllocation()
    {
        return alloc;
    }
    InteropTransaction ExportMemory() override;
    vk::DeviceMemory GetDeviceMemory() const;
    vk::DeviceSize GetOffset() const;
    vk::DeviceSize GetSize() const;
    [[nodiscard]] void *Map();
    void UnMap();
};
} // namespace VK

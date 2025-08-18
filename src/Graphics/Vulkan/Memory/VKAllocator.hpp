#pragma once
#include <Graphics/GPURef.hpp>
#include <pch.h>
namespace VK
{
class Device;
class Allocation;
enum AllocationMemoryType
{
    eAuto,
    ePreferGPU,
    ePreferCPU,
};
class Allocator : public Enable_GPURef_From_This<Allocator>
{
    // friend Allocation;
    VmaAllocator allocator;
    friend Allocation;

  public:
    Allocator(GPURef<VK::Device> Device);
    ~Allocator();
    // GPURef<Allocation> Allocate(vk::MemoryRequirements memRequirements, bool exportable);

    vk::Result CreateBuffer(const vk::BufferCreateInfo &CreateInfo, vk::Buffer &outBuffer, VK::Allocation &allocation);
    void DestroyBuffer(vk::Buffer buffer, VK::Allocation &allocation);
    vk::Result CreateImage(const vk::ImageCreateInfo &CreateInfo, vk::Image &outImage, VK::Allocation &allocation);
    void DestroyImage(vk::Image image, VK::Allocation &allocation);

  protected:
    [[nodiscard]] void *Map(const Allocation &alloc);
    void UnMap(const Allocation &alloc);

  private:
    // void FreeMemory(Allocation alloc);
};

} // namespace VK

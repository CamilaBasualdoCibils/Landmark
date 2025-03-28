#pragma once
#include <pch.h>
#include <VK/Devices/DeviceResource.h>
#include <GenericEnums.h>
class Buffer;
class CommandBuffer;
class Image : DeviceResource
{

public:
    struct ImageProperties
    {

        uvec3 dimensions;
        Format format = Format::RGBA_8_UINT;
        uint32_t layers = 1;
        uint32_t mipLevels = 1;
        uint32_t Samples = 1; // only relevant for images used as render attachments
        MemoryTillingType memoryTilling = MemoryTillingType::OPTIMAL;
        Flags<ImageUsage> usage = ImageUsage::TRANSFER_DST | ImageUsage::SAMPLED;
        Flags<MemoryProperties> memory_properties = MemoryProperties::DEVICE_LOCAL;

        // ImageLayouts
    };

private:
    vk::Image image;
    vk::DeviceMemory imageMemory;
    ImageProperties properties;
    size_t memorysize;

public:
    Image(const ImageProperties &ci);
    Image(const vk::Image &_image, const ImageProperties &ci);
    operator vk::Image() const { return GetVkImage(); }
    vk::Image GetVkImage() const { return image; }
    void Destroy() override;
    ImageProperties GetProperties() const { return properties; }
    void *MapMemory();
    void UnmapMemory();
    size_t GetMemorySize() const { return memorysize; }

    void TransferLayout(CommandBuffer &cmdbuf, ImageLayouts old_layout, ImageLayouts new_layout);
    void CopyFromBuffer(CommandBuffer &cmdbuf, Buffer &buffer);

private:
    void MakeImage();
};
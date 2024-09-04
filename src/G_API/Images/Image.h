#pragma once
#include <pch.h>
#include <G_API/Devices/DeviceResource.h>
#include <G_API/GenericEnums.h>
class Image : DeviceResource
{

public:
    struct ImageProperties
    {

        uvec3 dimensions;
        ColorFormat format = ColorFormat::RGBA8uint;
        uint32_t layers = 1;
        uint32_t mipLevels = 1;
        uint32_t Samples = 1; // only relevant for images used as render attachments
        MemoryTillingType memoryTilling = MemoryTillingType::OPTIMAL;
        vk::ImageUsageFlags usage = vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled;
        // ImageLayouts
    };

private:
    vk::Image image;
    vk::DeviceMemory imageMemory;
    ImageProperties properties;

public:
    Image(const ImageProperties &ci);
    Image(const vk::Image& _image,const ImageProperties& ci);
    operator vk::Image() const { return GetVkImage(); }
    vk::Image GetVkImage() const { return image; }
    void Destroy() override;
    ImageProperties GetProperties() const { return properties; }

private:
    void MakeImage();
};
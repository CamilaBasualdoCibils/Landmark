#include <pch.h>
#include "Image.h"
#include <GenericEnums.h>
class Sampler : DeviceResource
{

public:
    struct SamplerProperties
    {
        SamplerAddressModes addressModes[3] = {SamplerAddressModes::REPEAT};
        bool anisotropyEnable = false;
        BorderColor borderColor = BorderColor::FLOAT_OPAQUE_BLACK;
        std::pair<bool, CompareOp> compare = {false, CompareOp::LESS};
        Filter magFilter = Filter::NEAREST;
        Filter minFilter = Filter::NEAREST;
        float maxAnisotropy = 1;
        float MaxLod = 1;
        float MinLod = 0;
        float mipLodBias = 0.5;
        MipMapMode mipmapMode = MipMapMode::LINEAR;
    };

private:
    vk::Sampler sampler;
    SamplerProperties properties;

public:
    Sampler(const SamplerProperties& _properties):properties(_properties)
    {
        CreateSampler();
    }

    void Destroy() override {
        GetvkDevice().destroySampler(sampler);
    }
    vk::Sampler GetVkSampler() const {return sampler;}
    operator vk::Sampler() const {return sampler;}
    const SamplerProperties& GetProperties() const {return properties;}
private:
    void CreateSampler()
    {
        if (sampler != VK_NULL_HANDLE) {
            GetvkDevice().destroySampler(sampler);
        }
        
        vk::SamplerCreateInfo create_info;
        create_info.addressModeU = static_cast<vk::SamplerAddressMode>(properties.addressModes[0]);
        create_info.addressModeV = static_cast<vk::SamplerAddressMode>(properties.addressModes[1]);
        create_info.addressModeW = static_cast<vk::SamplerAddressMode>(properties.addressModes[2]);
        create_info.anisotropyEnable = properties.anisotropyEnable;
        create_info.borderColor = static_cast<vk::BorderColor>(properties.borderColor);
        create_info.compareEnable = properties.compare.first;
        create_info.compareOp = static_cast<vk::CompareOp>(properties.compare.second);
        create_info.magFilter = static_cast<vk::Filter>(properties.magFilter);
        create_info.minFilter = static_cast<vk::Filter>(properties.minFilter);
        create_info.maxAnisotropy = properties.maxAnisotropy;
        create_info.maxLod = properties.MaxLod;
        create_info.minLod = properties.MinLod;
        create_info.mipLodBias = properties.mipLodBias;
        create_info.mipmapMode = static_cast<vk::SamplerMipmapMode>(properties.mipmapMode);

        sampler = GetvkDevice().createSampler(create_info).value;
    }
};
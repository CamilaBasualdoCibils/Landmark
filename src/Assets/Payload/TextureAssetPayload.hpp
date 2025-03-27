#pragma once
#include <pch.h>
#include "AssetPayload.hpp"
#include <VK/Images/CombinedImageSampler.h>
#include <Assets/Descriptors/TextureAssetDescriptor.hpp>
class Asset;
class TextureAssetPayload :AssetPayload
{
private:
    /* data */
    std::vector<std::byte> cpu_data;
    std::optional<CombinedImageSampler> gpu_data;
    TextureAssetDescriptor descriptor;

public:
    TextureAssetPayload(Asset& asset);
    ~TextureAssetPayload();
};

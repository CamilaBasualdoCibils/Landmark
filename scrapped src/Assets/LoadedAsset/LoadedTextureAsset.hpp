#pragma once
#include <pch.h>
#include "LoadedAsset.hpp"
#include <VK/Images/CombinedImageSampler.h>
#include <Assets/Descriptors/TextureAssetDescriptor.hpp>
#include <Assets/Asset.hpp>
class Asset;
class LoadedTextureAsset : public LoadedAsset
{
private:
    /* data */
    using ImageData = std::vector<std::byte>;
    std::optional<ImageData> cpu_data;
    std::optional<CombinedImageSampler> gpu_data;
    const TextureAssetDescriptor descriptor;
    const Asset asset;

    static ImageData LoadImageData(const Asset &asset, const TextureAssetDescriptor &descriptor);
    static CombinedImageSampler CreateVkImage(const std::vector<std::byte> &, const TextureAssetDescriptor &descriptor);

public:
    LoadedTextureAsset(Asset &asset);
    ~LoadedTextureAsset() override {}

    const ImageData &GetImageData() const;
    const CombinedImageSampler &GetGpuImage() const;
    const TextureAssetDescriptor &GetDescriptor() const { return descriptor; }

    ImageData LoadNewImageData() const;
    CombinedImageSampler LoadNewGpuImage() const;
};

#pragma once
#include <pch.h>
#include "AssetInspector.hpp"
#include <VK/Images/CombinedImageSampler.h>
class LoadedTextureAsset;
class TextureAssetInspector : public AssetInspector {
    std::shared_ptr<LoadedTextureAsset> texture;
    std::optional<CombinedImageSampler> vkimage;
    std::optional<vk::DescriptorSet> imgui_ds;
    std::optional<Sampler> sampler;
    TextureAssetDescriptor original_descriptor,modified_descriptor;
    bool modified = false;
    private:
    void RebuildDescriptorSet();
    void SaveChanges();
    public:
    void SetAsset(const Asset& asset) override;
    void DrawWindowContents() override;
    ~TextureAssetInspector() override;
};
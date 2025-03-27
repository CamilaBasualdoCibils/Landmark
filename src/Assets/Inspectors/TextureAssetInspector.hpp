#pragma once
#include <pch.h>
#include "AssetInspector.hpp"
#include <VK/Images/CombinedImageSampler.h>
class TextureAssetInspector : public AssetInspector {
    std::optional<CombinedImageSampler> vkimage;
    vk::DescriptorSet imgui_ds;
    public:
    void SetAsset(const Asset& asset) override;
    void DrawContents() override;
};
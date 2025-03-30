#pragma once
#include <pch.h>
#include <Assets/Inspectors/AssetInspector.hpp>
#include <VK/Images/CombinedImageSampler.h>
class LoadedTextureAsset;
class TextureAssetInspector : public AssetInspector {
    
    std::shared_ptr<LoadedTextureAsset> texture;
    std::optional<CombinedImageSampler> vkimage;
    std::optional<vk::DescriptorSet> imgui_ds;
    std::optional<Sampler> sampler;
    AssetDescriptor original_descriptor;
    TextureAssetDescriptor modified_tex_descriptor;
    struct TextureInspectorVars {

    };
    TextureInspectorVars texture_inspect_vars;
    struct SpriteInspectorVars {
        uint64_t currently_selected_sprite_ID = -1;
        enum class GridRates {
            eNONE = 0,
            ePIXEL = 1,eTexcoord = 2
        };
        GridRates grid_rate_mode = GridRates::ePIXEL;
        vec2 grid_rate = {1,1};
        enum class SnapMode {
            eNONE = 0,ePIXEL = 1,eGRID = 2
        };
        SnapMode snapping = SnapMode::eNONE;
    };
    SpriteInspectorVars sprite_inspector_vars;
    bool modified = false;
    private:
    void RebuildDescriptorSet();
    void SaveChanges();
    void DrawTextureView();
    void DrawSpriteSheetView();
    vec2 Uv2Screen(vec2 v);
    vec2 Screen2UV(vec2 s);
    public:
    void SetAsset(const Asset& asset) override;
    void DrawWindowContents() override;
    ~TextureAssetInspector() override;
};
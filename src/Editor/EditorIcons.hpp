#pragma once
#include <pch.h>
#include <VK/Images/CombinedImageSampler.h>
#include <Assets/Asset.hpp>
class EditorIcons {
    
    public:
    struct Icon_Data {
        vec2 uv_min,uv_max;
        //uvec2 index2D;
        //uint64_t index1D;
    };
    struct IconSheet {
        std::optional<CombinedImageSampler> image;
        ImTextureID imgui_image;
        std::optional<Asset> asset;
        uint32_t icon_count;
        std::unordered_map<std::string,Icon_Data> icons;
    };
    
    static void LoadIconSheet(Asset& asset);
using Icon = std::pair<Icon_Data,ImTextureID>;
    static Icon GetIcon(const std::string& _icon_sheet,const std::string& _icon);
    private:
    static inline Log logger = Log("EditorIcons");
    static inline std::unordered_map<std::string,IconSheet> icon_sheets;
};
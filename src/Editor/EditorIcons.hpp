#pragma once
#include <pch.h>
#include <VK/Images/CombinedImageSampler.h>
#include <Assets/Asset.hpp>
class EditorIcons {
    public:
    struct Icon {
        vec2 uv_min,uv_max;
        uvec2 index2D;
        uint64_t index1D;
    };
    struct IconSheet {
        CombinedImageSampler image;
        Asset asset;
        uvec2 icon_count;
        std::unordered_map<std::string,Icon> icons;
    };

    static inline std::unordered_map<std::string,IconSheet> icon_sheets;

    static void LoadIconSheet(Asset& asset);
};
#pragma once
#include <pch.h>
#include <IO/File.h>
#include "AssetJsonUtils.hpp"
#include "AssetEnums.hpp"
#include <Types/Flags.h>
struct TextureAssetInfo
{
    struct SpriteInfo
    {
        std::string name;
        vec2 uv_min, uv_max;
    };
    struct SpriteSheetInfo
    {
        std::vector<SpriteInfo> sprites;
    };
    enum TextureFormat
    {
        e16BIT = 1 << 0,
        eHDR = 1 << 1
    };
    using TextureFormatFlags = uint32_t;
    ivec2 dimensions;
    int32_t channel_count;
    TextureFormatFlags format = 0;
    SpriteSheetInfo spritesheet;
    static TextureAssetInfo FromTextureFile(const File &file);
};
struct TextureAssetParameters
{
    Flags<AssetAvailability> availability = AssetAvailability::eGPU;
};

struct TextureAssetDescriptor
{
    TextureAssetParameters parameters;
    TextureAssetInfo info;
    ASSET_JSON_VAR(info_dimensions, "Dimensions");
    ASSET_JSON_VAR(info_dimensions_x, "x");
    ASSET_JSON_VAR(info_dimensions_y, "y");
    ASSET_JSON_VAR(info_channel_count, "Channel count");
    ASSET_JSON_VAR(availability, "Availability");


    static TextureAssetDescriptor FromJson(const Json &file);
    Json ToJson() const;
    bool DisplayProperties();
};

#pragma once
#include <pch.h>
#include <IO/File.h>
#include <Assets/AssetJsonUtils.hpp>
#include <Assets/AssetEnums.hpp>
#include <Types/Flags.hpp>
struct TextureAssetInfo
{

    enum TextureFormat
    {
        e8BIT,
        e16BIT,
        eHDR 
    };
    ivec2 dimensions;
    int32_t channel_count;
    TextureFormat format = TextureFormat::e8BIT;

    static TextureAssetInfo FromTextureFile(const File &file);
};
struct TextureAssetParameters
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
    SpriteSheetInfo spritesheet;
    Flags<AssetAvailability> availability = AssetAvailability::eGPU;
};

struct TextureAssetDescriptor
{
    TextureAssetParameters parameters;
    TextureAssetInfo info;
    ASSET_JSON_VAR(info_dimensions, "Dimensions");
    ASSET_JSON_VAR(info_channel_count, "Channel count");
    ASSET_JSON_VAR(availability, "Availability");
    ASSET_JSON_VAR(spritesheet, "Sprite sheet parameters");



    static TextureAssetDescriptor FromJson(const Json &file);
    Json ToJson() const;
    bool DisplayProperties();
};

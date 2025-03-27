#pragma once
#include <pch.h>
#include "LoadedAsset.hpp"
#include <Types/shared_ctrl_ptr.hpp>
#include <Assets/Payload/TextureAssetPayload.hpp>
class LoadedTextureAsset : LoadedAsset
{
private:
    shared_ctrl_ptr<TextureAssetPayload> payload;
    /* data */
public:
    LoadedTextureAsset(Asset &asset);
    ~LoadedTextureAsset();
};

LoadedTextureAsset::~LoadedTextureAsset()
{
}

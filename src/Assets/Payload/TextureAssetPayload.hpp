#pragma once
#include <pch.h>
#include "AssetPayload.hpp"
class Asset;
class TextureAssetPayload :AssetPayload
{
private:
    /* data */
public:
    TextureAssetPayload(Asset& asset);
    ~TextureAssetPayload();
};

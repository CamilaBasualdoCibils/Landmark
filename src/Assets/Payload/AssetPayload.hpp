#pragma once
#include <pch.h>
class Asset;
struct AssetPayload
{
    AssetPayload(Asset& asset);
virtual ~AssetPayload();
};
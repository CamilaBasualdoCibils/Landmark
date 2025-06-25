#include "AssetInspector.hpp"


void AssetInspector::SetAsset(const Asset &_asset)
{
    asset.emplace(_asset);
}

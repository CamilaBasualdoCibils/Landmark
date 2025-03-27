#include "AssetInspector.hpp"

void AssetInspector::Draw()
{
    if (!open)
        return;
    ImGui::Begin(name.c_str(), &open);
    DrawContents();
    ImGui::End();
}

void AssetInspector::SetAsset(const Asset &_asset)
{
    *asset = _asset;
}

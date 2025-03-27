#include "AssetManager.hpp"
#include <Editor/Editor.h>
#include "AssetExplorer.hpp"

void AssetManager::Init()
{
    Editor::GetInstance()->GetMainToolGroup().GetPanelsGroup().PushObject<AssetExplorer>().SetIsOpen(true);
}

bool AssetManager::IsAssetLoaded(Asset &asset)
{
    return loaded_assets.contains(GetAssetIdentifier(asset));
}


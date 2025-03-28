#include "AssetManager.hpp"
#include <Editor/Editor.h>
#include "AssetExplorer.hpp"
#include <Core/App.h>
#include "Asset.hpp"
#include "Inspectors/TextureAssetInspector.hpp"
void AssetManager::Init()
{
    Editor::GetInstance()->GetMainToolGroup().GetPanelsGroup().PushObject<AssetExplorer>().SetIsOpen(true);
}



bool AssetManager::IsAssetLoaded(Asset &asset)
{
    return loaded_assets.contains(GetAssetIdentifier(asset));
}

decltype(AssetManager::INSTANCE) AssetManager::GetInstance()
{
    if (!INSTANCE)
        INSTANCE = App::GetInstance()->GetModule<AssetManager>();
    return INSTANCE;
}

void AssetManager::ReloadAsset(Asset &asset)
{
}

constexpr std::string AssetManager::GetAssetIdentifier(Asset &asset)
{
   return asset.GetPath();
}


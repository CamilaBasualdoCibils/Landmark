#include "AssetManager.hpp"
#include <Editor/Editor.h>
#include "AssetExplorer.hpp"
#include <Core/App.h>
#include "Asset.hpp"
#include "Inspectors/Texture/TextureAssetInspector.hpp"
#include <Assets/Descriptors/AssetDescriptor.hpp>
void AssetManager::Init()
{
    Editor::GetInstance()->GetMainToolGroup().GetPanelsGroup()->PushObject<AssetExplorer>()->SetIsOpen(true);
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
    logger.Error("Asset Reloading not implemented");
}

void AssetManager::OverwriteAssetDescriptor(Asset& asset,const AssetDescriptor& desc)
{

asset.GetDescriptorFile().WriteText(desc.ToJson().dump(4));
logger.Debug("Asset " + asset.GetPath() + "'s descriptor overwritten");
ReloadAsset(asset);
}

std::string AssetManager::GetAssetIdentifier(Asset &asset)
{
   return asset.GetPath();
}


#pragma once
#include <pch.h>
#include <Core/AppModule.h>
//#include "LoadedAsset/LoadedAsset.hpp"
class AssetInspector;
class Asset;
class LoadedAsset;
class AssetManager : public AppModule
{
private:
    void Init() override;
    std::map<std::string, std::shared_ptr<LoadedAsset>> loaded_assets;


    static inline std::shared_ptr<AssetManager> INSTANCE;

public:
    bool IsAssetLoaded(Asset &asset);
    static decltype(INSTANCE) GetInstance();
    template <typename T> //, std::enable_if_t<std::is_base_of_v<AssetPayload,T>>>
    std::shared_ptr<T> GetOrLoadAsset(Asset &asset);
    void ReloadAsset(Asset &asset);
    constexpr static std::string GetAssetIdentifier(Asset &asset);
    // std::map<std::string
};

template <typename T> //,  std::enable_if_t<std::is_base_of_v<AssetPayload,T>>>
inline std::shared_ptr<T> AssetManager::GetOrLoadAsset(Asset &asset)
{
    std::shared_ptr<LoadedAsset> base;
    const auto Asset_ID = GetAssetIdentifier(asset);
    if (!loaded_assets.contains(Asset_ID))
        loaded_assets[Asset_ID] = std::dynamic_pointer_cast<LoadedAsset>(std::make_shared<T>(asset));
    // loaded_assets.insert(std::make_pair(Asset_ID,std::make_shared<T>(asset)));

    base = loaded_assets.at(Asset_ID);

    return std::dynamic_pointer_cast<T>(base);
}

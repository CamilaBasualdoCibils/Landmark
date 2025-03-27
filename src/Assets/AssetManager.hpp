#pragma once
#include <pch.h>
#include <Core/AppModule.h>
#include "Payload/AssetPayload.hpp"
#include "Asset.hpp"
class AssetManager : public AppModule {
    void Init() override;
    std::map<std::string,std::shared_ptr<AssetPayload>> loaded_assets;

    public:
    bool IsAssetLoaded(Asset& asset );
    template <typename T, std::enable_if_t<std::is_base_of_v<AssetPayload,T>>>
    std::shared_ptr<T> GetOrLoadAsset(Asset& asset);
    constexpr static std::string GetAssetIdentifier(Asset& asset) {return asset.GetPath();}
   // std::map<std::string
};

template <typename T,  std::enable_if_t<std::is_base_of_v<AssetPayload,T>>>
inline std::shared_ptr<T> AssetManager::GetOrLoadAsset(Asset &asset)
{
    //std::shared_ptr<AssetPayload>
}

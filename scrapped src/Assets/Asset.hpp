#pragma once
#include <pch.h>
#include <IO/File.h>
#include "Descriptors/AssetDescriptor.hpp"
#include <Assets/AssetManager.hpp>
class LoadedAsset;
class Asset : public File
{
public:
    const static inline std::string descriptor_file_extension = "desc.json";

private:
    const static inline std::unordered_map<AssetType, std::vector<std::string>> known_type_extensions = {{AssetType::eTEXTURE, {"png", "jpeg", "jpg"}},
                                                                                                         {AssetType::eMESH, {"obj", "fbx", "gltf"}},
                                                                                                        {AssetType::eASEPRITE,{"aseprite","ase"}}};

public:
    mutable std::optional<AssetDescriptor> descriptor;

    Asset(const std::string &file);
    void Parse();
    void GenerateDescriptorFile();
    File GetRawFile();
    File GetDescriptorFile();

    AssetType GetType();
    const AssetDescriptor &GetDescriptor();
    
    template <typename T>//, std::enable_if_t<std::is_base_of_v<LoadedAsset,T>>>
    std::shared_ptr<T> Load();
    private:


private:
    void CheckForUpdate();
    AssetType AssetTypeFromExtension() const;
    static inline Log LOGGER = Log("Asset");
};

template <typename T>//, std::enable_if_t<std::is_base_of_v<LoadedAsset,T>>>
inline std::shared_ptr<T> Asset::Load()
{
    return AssetManager::GetInstance()->GetOrLoadAsset<T>(*this);
}

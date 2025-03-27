#pragma once
#include <pch.h>
#include <IO/File.h>
#include "AssetDescriptor.hpp"
class Asset : public File
{
public:
    const static inline std::string descriptor_file_extension = "desc.json";

private:
    const static inline std::unordered_map<AssetType, std::vector<std::string>> known_type_extensions = {{AssetType::eTEXTURE, {"png", "jpeg", "jpg"}},
                                                                                                         {AssetType::eMESH, {"obj", "fbx", "gltf"}}};

public:
    mutable std::optional<AssetDescriptor> descriptor;

    Asset(const std::string &file);
    void Parse();
    void GenerateDescriptorFile();
    File GetRawFile();
    File GetDescriptorFile();

    AssetType GetType();
    const AssetDescriptor &GetDescriptor();

private:
    void CheckForUpdate();
    AssetType AssetTypeFromExtension() const;
    static inline Log LOGGER = Log("Asset");
};
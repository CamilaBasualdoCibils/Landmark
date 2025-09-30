#pragma once
#include "Asset.hpp"
#include "IO/File/File.hpp"
#include "IO/File/Folder.hpp"
#include "misc/Singleton.hpp"
#include <pch.h>
class AssetRegistry : public Singleton<AssetRegistry>
{
    using AssetPath = std::filesystem::path;
    using FilePath = std::filesystem::path;
    struct AssetInfo
    {
        std::string Name;
        std::optional<std::vector<FilePath>> FileReferences;
    };

    std::unordered_map<AssetPath, AssetInfo> parsedAssets;

    void ParseFile(File file);
    void ImportAsset(File file, File &outImported, Asset &outAsset);
  public:
    AssetRegistry();
    ~AssetRegistry();
};
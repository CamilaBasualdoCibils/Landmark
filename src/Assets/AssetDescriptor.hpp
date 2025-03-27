#pragma once
#include <pch.h>
#include "TextureAssetDescriptor.hpp"
#include "AssetJsonUtils.hpp"
enum class AssetType {
    eINVALID,
    eUNKNOWN,
    eTEXTURE,
    eMESH,
};

const static inline std::unordered_map<AssetType,std::string> AssetTypeString = 
{{AssetType::eINVALID,"Invalid"},
{AssetType::eUNKNOWN,"Unknown"},
{AssetType::eTEXTURE,"Texture"},
{AssetType::eMESH, "Mesh"}};
const static inline std::unordered_map<std::string,AssetType> AssetTypeStringReverse = [](){
    std::unordered_map<std::string,AssetType> a;
    for (const auto& e :AssetTypeString ) {
        a.insert(std::make_pair(e.second,e.first));
    }
    return a;
}();
struct AssetDescriptor{
    AssetType type = AssetType::eINVALID;
    ASSET_JSON_VAR(type,"Type");
    std::filesystem::file_time_type last_write_time;
    ASSET_JSON_VAR(last_write_time,"Last write time");
    std::string imported_path;
    ASSET_JSON_VAR(imported_path,"Imported Path");
    std::variant<TextureAssetDescriptor> descriptor_data;
    ASSET_JSON_VAR(texture_data,"Texture parameters");

    Json ToJson() const;
    void FromJson(const Json& json);
    static std::filesystem::file_time_type GetLastWriteTimeFromJson(const Json& json);

   static std::string formatTime( std::filesystem::file_time_type);
   static std::filesystem::file_time_type unformatTime(const std::string&);

   const TextureAssetDescriptor& GetTextureDescription() const {return std::get<TextureAssetDescriptor>(descriptor_data);}
};
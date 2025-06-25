#include "Asset.hpp"

Asset::Asset(const std::string &file) : File(file)
{
    if (std::filesystem::path(file).is_absolute())
    {
        LOGGER.Error("Invalid Asset Path: " + file);
        return;
    }
    if (GetFullExtension() == descriptor_file_extension)
    {
        LOGGER.Error("Cannot create asset from descriptor file. path must be non descriptor file");
        return;
    }
    Parse();
}

void Asset::Parse()
{
    if (!GetRawFile().Exists())
    {
        LOGGER.Error("Invalid Asset Path. File does not exist");
        return;
    }
    const File &descriptor_file = GetDescriptorFile();
    if (!descriptor_file.Exists())
    {
        LOGGER.Debug("Generating descriptor for file without one: " + GetPath());
        GenerateDescriptorFile();
        return;
    }
    if (!descriptor.has_value()) descriptor.emplace();
    const Json json = Json::parse(GetDescriptorFile().ReadText());
    descriptor->FromJson(json);
}

void Asset::GenerateDescriptorFile()
{
    descriptor.emplace();
    descriptor->type = AssetTypeFromExtension();
    descriptor->last_write_time = GetLastWriteTime();
    switch (descriptor->type)
    {
    case AssetType::eTEXTURE:
    {
        TextureAssetDescriptor texture_data{};
        texture_data.info = TextureAssetInfo::FromTextureFile(*this);
        descriptor->descriptor_data = texture_data;
    }
    /* code */
    break;

    default:
        break;
    }
    Json asset_descriptor_json = descriptor->ToJson();

    File asset_descriptor_file = GetPath() + "." + descriptor_file_extension;

    asset_descriptor_file.WriteText(asset_descriptor_json.dump(4));
}

File Asset::GetRawFile()
{
    return File(GetPath());
}

File Asset::GetDescriptorFile()
{
    return File(GetPath() + '.' + descriptor_file_extension);
}

AssetType Asset::GetType()
{
    CheckForUpdate();
    return descriptor->type;
}

const AssetDescriptor&  Asset::GetDescriptor()
{
    CheckForUpdate();
    return *descriptor;
}

void Asset::CheckForUpdate()
{
   
    if (!GetDescriptorFile().Exists()) GenerateDescriptorFile();
    Json new_json = GetDescriptorFile().ReadJson();
    if (!descriptor.has_value() || AssetDescriptor::unformatTime(new_json[AssetDescriptor::last_write_time__identifier]) > descriptor->last_write_time)
    {
        // outdated. must reparse

        Parse();
    }
}

AssetType Asset::AssetTypeFromExtension() const
{
    const std::string extension = GetLastExtension();
    for (const auto &type : known_type_extensions)
    {
        for (const auto &ext : type.second)
        {
            if (ext == extension)
                return type.first;
        }
    }
    return AssetType::eUNKNOWN;
}

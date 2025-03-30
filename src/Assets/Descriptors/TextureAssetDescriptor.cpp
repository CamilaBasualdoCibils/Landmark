#include "TextureAssetDescriptor.hpp"
#include <JSON/JsonUtils.hpp>
TextureAssetInfo TextureAssetInfo::FromTextureFile(const File &file)
{
    TextureAssetInfo info;
    std::string path = file.GetPath();

    int ok = stb::stbi_info(path.c_str(), &info.dimensions.x, &info.dimensions.y, &info.channel_count);
    if (stb::stbi_is_16_bit(path.c_str()))
        info.format = TextureFormat::e16BIT;
    else if (stb::stbi_is_hdr(path.c_str()))
        info.format = TextureFormat::eHDR;
    return info;
}

TextureAssetDescriptor TextureAssetDescriptor::FromJson(const Json &json)
{
    TextureAssetDescriptor tad{};
    const Json &dimension_json = json[info_dimensions__identifier];
    tad.info.dimensions = JsonUtils::Vec2FromJson(dimension_json, JsonUtils::XYZW);
    tad.info.channel_count = json[info_channel_count__identifier].get<int>();
    if (json.contains(spritesheet__identifier))
    {
        const Json &sprite_sheet_parameters = json[spritesheet__identifier];
        for (const auto &spr : sprite_sheet_parameters)
        {
            TextureAssetParameters::SpriteInfo spr_info;

            spr_info.name = spr["name"].get<std::string>();
            spr_info.uv_max = JsonUtils::Vec2FromJson(spr["uv max"], JsonUtils::XYZW);
            spr_info.uv_min = JsonUtils::Vec2FromJson(spr["uv min"], JsonUtils::XYZW);
            tad.parameters.spritesheet.sprites.push_back(spr_info);
        }
    }

    return tad;
}

Json TextureAssetDescriptor::ToJson() const
{
    Json json = {
        {info_dimensions__identifier,JsonUtils::Vec2ToJson(info.dimensions,JsonUtils::JsonVecFormat::XYZW)},
        {info_channel_count__identifier, info.channel_count},
        {spritesheet__identifier, {}}};

    auto &sprite_sheet_parameters = json[spritesheet__identifier];
    for (const auto &spr : parameters.spritesheet.sprites)
    {
        sprite_sheet_parameters.push_back({
            {"name",spr.name},
            {"uv min",JsonUtils::Vec2ToJson(spr.uv_min,JsonUtils::JsonVecFormat::XYZW)},
            {"uv max", JsonUtils::Vec2ToJson(spr.uv_max,JsonUtils::JsonVecFormat::XYZW)}
        });
      
    }
    return json;
}

bool TextureAssetDescriptor::DisplayProperties()
{
    bool modified = false;
    ImGui::BeginChild("info", ImVec2(0, 0), ImGuiChildFlags_AutoResizeY, ImGuiWindowFlags_NoBackground);

    ImGui::Text("Dimensions: %d, %d", info.dimensions.x, info.dimensions.y);
    ImGui::Text("Channel count: %d", info.channel_count);

    ImGui::Text("16 Bit: %s", (info.format & TextureAssetInfo::TextureFormat::e16BIT) ? "true" : "false");
    ImGui::Text("HDR: %s", (info.format & TextureAssetInfo::TextureFormat::eHDR) ? "true" : "false");
    ImGui::Text("Sprite Count: %ld",parameters.spritesheet.sprites.size());
    ImGui::EndChild();
    ImGui::Separator();
    ImGui::BeginChild("Parameters");
    modified |= AssetAvailabilityInspector(parameters.availability);
    ImGui::EndChild();
    return modified;
}

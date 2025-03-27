#include "TextureAssetDescriptor.hpp"

TextureAssetInfo TextureAssetInfo::FromTextureFile(const File &file)
{
    TextureAssetInfo info;
    std::string path = file.GetPath();

    int ok = stb::stbi_info(path.c_str(), &info.dimensions.x, &info.dimensions.y, &info.channel_count);
    if (stb::stbi_is_16_bit(path.c_str())) info.format = TextureFormat::e16BIT;
    else if (stb::stbi_is_hdr(path.c_str())) info.format = TextureFormat::eHDR;
    return info;
}

TextureAssetDescriptor TextureAssetDescriptor::FromJson(const Json &json)
{
    TextureAssetDescriptor tad{};
    const Json &dimension_json = json[info_dimensions__identifier];
    tad.info.dimensions.x = dimension_json[info_dimensions_x__identifier].get<double>();
    tad.info.dimensions.y = dimension_json[info_dimensions_y__identifier].get<double>();
    tad.info.channel_count = json[info_channel_count__identifier].get<int>();
    return tad;
}

Json TextureAssetDescriptor::ToJson() const
{
    Json json = {
        {info_dimensions__identifier, { 
            {info_dimensions_x__identifier, info.dimensions.x}, 
            {info_dimensions_y__identifier, info.dimensions.y}}},
            {info_channel_count__identifier,info.channel_count},
        {"sprite sheet parameters", {}}};

    auto &sprite_sheet_parameters = json["sprite sheet parameters"];
    for (const auto &spr : info.spritesheet.sprites)
    {
        sprite_sheet_parameters[spr.name] = {
            {"uv min", {{"x", spr.uv_min.x}, {"y", spr.uv_min.y}}},
            {"uv max", {{"x", spr.uv_max.x}, {"y", spr.uv_max.y}}}};
    }
    return json;
}

bool TextureAssetDescriptor::DisplayProperties()
{
    bool modified = false;
    ImGui::BeginChild("info",ImVec2(0,0),ImGuiChildFlags_AutoResizeY,ImGuiWindowFlags_NoBackground);
    
    ImGui::Text("Dimensions: %d, %d",info.dimensions.x,info.dimensions.y);
    ImGui::Text("Channel count: %d",info.channel_count);

    ImGui::Text("16 Bit: %s",(info.format & TextureAssetInfo::TextureFormat::e16BIT)?"true":"false");
    ImGui::Text("HDR: %s",(info.format &  TextureAssetInfo::TextureFormat::eHDR)?"true":"false");
    
    ImGui::EndChild();
    ImGui::Separator();
    ImGui::BeginChild("Parameters");
    modified |= AssetAvailabilityInspector(parameters.availability);
    ImGui::EndChild();
    return modified;
}

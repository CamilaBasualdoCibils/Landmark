#include "EditorIcons.hpp"
#include <Assets/Asset.hpp>
#include <Assets/LoadedAsset/LoadedTextureAsset.hpp>
void EditorIcons::LoadIconSheet(Asset &asset)
{
    if (icon_sheets.contains(asset.GetName())) return;
    IconSheet& sheet = icon_sheets[asset.GetName()];
    sheet.image = asset.Load<LoadedTextureAsset>()->LoadNewGpuImage();
    sheet.asset = asset;
    sheet.imgui_image = (ImTextureID)ImGui_ImplVulkan_AddTexture((vk::Sampler)sheet.image->GetSampler(),(vk::ImageView)sheet.image->GetImageView(),VkImageLayout::VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    const TextureAssetDescriptor& texture_desc = asset.GetDescriptor().GetTextureDescription();

    for (const auto& sprite: texture_desc.parameters.spritesheet.sprites) {
        Icon_Data& icon = sheet.icons[sprite.name];
        icon.uv_min = sprite.uv_min;
        icon.uv_max = sprite.uv_max;
        sheet.icon_count++;
    }
    logger.Debug("Icon sheet " + asset.GetName() + " loaded with " + std::to_string(sheet.icon_count) + " icons");
}

EditorIcons::Icon EditorIcons::GetIcon(const std::string& _icon_sheet,const std::string& icon_name)
{
    if (!icon_sheets.contains(_icon_sheet)) {
        logger.Error("Invalid Icon sheet name: " + _icon_sheet);
        return{};
    }
    const IconSheet& sheet = icon_sheets[_icon_sheet];
    if (!sheet.icons.contains(icon_name)) {
        logger.Error("Invalid Icon name: " + icon_name + " in sheet:" + _icon_sheet );
        return{};
    }

    const Icon_Data& icon = sheet.icons.at(icon_name);

    return {icon,sheet.imgui_image};
}

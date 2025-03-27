#include "TextureAssetPayload.hpp"
#include <Assets/Asset.hpp>
#include <Assets/Descriptors/TextureAssetDescriptor.hpp>
TextureAssetPayload::TextureAssetPayload(Asset &asset) : AssetPayload(asset)
{
    descriptor = asset.GetDescriptor().GetTextureDescription();

    void *data = nullptr;

    switch (descriptor.info.format)
    {
    case TextureAssetInfo::TextureFormat::e8BIT:
        data = stb::stbi_load(asset.GetPath().c_str(), nullptr, nullptr, nullptr, descriptor.info.channel_count);
        break;
    case TextureAssetInfo::TextureFormat::e16BIT:
        data = stb::stbi_load_16(asset.GetPath().c_str(), nullptr, nullptr, nullptr, descriptor.info.channel_count);
        break;
    case TextureAssetInfo::TextureFormat::eHDR:
        data = stb::stbi_loadf(asset.GetPath().c_str(), nullptr, nullptr, nullptr, descriptor.info.channel_count);

        break;
    default:
    throw "unknown error";
        break;
    }
    
    if (data) stb::stbi_image_free(data);
}

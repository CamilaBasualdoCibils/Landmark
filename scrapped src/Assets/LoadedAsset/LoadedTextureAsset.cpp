#include "LoadedTextureAsset.hpp"
#include <Assets/Asset.hpp>
#include <Assets/Descriptors/TextureAssetDescriptor.hpp>
#include <VK/Buffers/Buffer.h>
#include <Core/App.h>
std::vector<std::byte> LoadedTextureAsset::LoadImageData(const Asset& asset,const TextureAssetDescriptor& descriptor)
{
     std::vector<std::byte> data;
     size_t data_size = descriptor.info.dimensions.x * descriptor.info.dimensions.y*4;
     if (descriptor.info.format == TextureAssetInfo::e16BIT) data_size *=2;
     if (descriptor.info.format == TextureAssetInfo::eHDR) data_size *=4;

     data.resize(data_size);
    void *stb_data = nullptr;


    int x_thr,y_thr,ch_thr;
    switch (descriptor.info.format)
    {
    case TextureAssetInfo::TextureFormat::e8BIT:
    stb_data = stb::stbi_load(asset.GetPath().c_str(), &x_thr, &y_thr, &ch_thr, 4);
        break;
    case TextureAssetInfo::TextureFormat::e16BIT:
    stb_data = stb::stbi_load_16(asset.GetPath().c_str(), &x_thr, &y_thr, &ch_thr,4);
        break;
    case TextureAssetInfo::TextureFormat::eHDR:
    stb_data = stb::stbi_loadf(asset.GetPath().c_str(), &x_thr, &y_thr, &ch_thr, 4);

        break;
    default:
        throw "unknown error";
        break;
    }
    std::memcpy(data.data(),stb_data,data_size);
    stb::stbi_image_free(stb_data);

    return data;
}
CombinedImageSampler LoadedTextureAsset::CreateVkImage(const std::vector<std::byte> &data, const TextureAssetDescriptor &descriptor)
{
    CombinedImageSampler::CISProperties image_prop;
    image_prop.image.dimensions = {descriptor.info.dimensions.x, descriptor.info.dimensions.y, 1};
    Format image_formats_8_bit[] = {Format::R_8_SRGB,
                                    Format::RG_8_SRGB,
                                    Format::RGB_8_SRGB,
                                    Format::RGBA_8_SRGB};


    image_prop.image.format = Format::RGBA_8_SRGB;
    CombinedImageSampler image(image_prop);

    Buffer::BufferProperties staging_buffer_prop;
    staging_buffer_prop.size = image.GetImage().GetMemorySize();
    staging_buffer_prop.memory_properties = {MemoryProperties::HOST_VISIBLE, MemoryProperties::HOST_COHERENT, MemoryProperties::DEVICE_LOCAL};
    staging_buffer_prop.usage_flags = BufferUsage::TRANSFER_SRC;
    Buffer staging_buffer(staging_buffer_prop);
    staging_buffer.InsertData(0,data.size(),(void*)data.data());

    {
        CommandPool pool(App::GetInstance()->GetMainDevice()->GetGraphicsQueue());
        SingleUseCommandBuffer dispatch(pool);

        image.GetImage().TransferLayout(dispatch,ImageLayouts::UNDEFINED,ImageLayouts::TRANSFER_DST);
        image.GetImage().CopyFromBuffer(dispatch,staging_buffer);
        image.GetImage().TransferLayout(dispatch,ImageLayouts::TRANSFER_DST,ImageLayouts::SHADER_READ_ONLY);
    }
    staging_buffer.Destroy();
    return image;
}
LoadedTextureAsset::LoadedTextureAsset(Asset &_asset) : LoadedAsset(_asset), asset(_asset), descriptor(_asset.GetDescriptor().GetTextureDescription())
{
    if (descriptor.parameters.availability == AssetAvailability::eNONE) return;
    void *data = nullptr;

    auto loaded_data = LoadImageData(asset,descriptor);
    if (descriptor.parameters.availability & AssetAvailability::eCPU) {
        cpu_data = loaded_data;
    }
    if (descriptor.parameters.availability & AssetAvailability::eGPU) {
        gpu_data.emplace(CreateVkImage(loaded_data,descriptor));
    }
    
}

const LoadedTextureAsset::ImageData &LoadedTextureAsset::GetImageData() const
{
   if (!cpu_data.has_value()) throw "Invalid use. Availablility is not set to CPU. Call LoadNewImageData() or set availability to CPU";
   return *cpu_data;
}

const CombinedImageSampler &LoadedTextureAsset::GetGpuImage() const
{
    if (!gpu_data.has_value()) throw "Invalid use. Availablility is not set to GPU. Call LoadNewGPUImage() or set availability to GPU";
    return *gpu_data;
}

LoadedTextureAsset::ImageData LoadedTextureAsset::LoadNewImageData() const
{
    if (cpu_data.has_value()) return *cpu_data;
    return LoadImageData(asset,descriptor);
}

CombinedImageSampler LoadedTextureAsset::LoadNewGpuImage() const
{
    if (cpu_data.has_value())
        return CreateVkImage(*cpu_data,descriptor);
        else
        
    return CreateVkImage(LoadNewImageData(),descriptor);
}

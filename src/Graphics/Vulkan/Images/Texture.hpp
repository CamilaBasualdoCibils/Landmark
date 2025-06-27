#pragma once
#include "Graphics/Interop/Interop.hpp"
#include "Graphics/Vulkan/Images/Image.hpp"
#include "Graphics/Vulkan/Images/ImageView.hpp"
#include "Graphics/Vulkan/Images/Sampler.hpp"
#include <vulkan/vulkan_handles.hpp>
namespace VK
{
struct TextureProperties
{
    ImageProperties ImageProp;
    ImageViewProperties ImageViewProp;
    SamplerProperties SamplerProp;
};
class Texture : public InteropGiver
{
    VK::Image image;
    VK::ImageView imageView;
    VK::Sampler sampler;

  public:
    Texture(const TextureProperties& Properties);

    InteropTransaction ExportMemory() const override {return image.ExportMemory();}

    const auto& GetImage() const {return image;}
    auto& GetImage() {return image;}

        const auto& GetImageView() const {return imageView;}
    auto& GetImageView() {return imageView;}

        const auto& GetSampler() const {return sampler;}
    auto& GetSampler() {return sampler;}

};
} // namespace VK

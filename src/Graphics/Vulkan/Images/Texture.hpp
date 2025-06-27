#pragma once
#include "Graphics/Interop/Interop.hpp"
#include "Graphics/Vulkan/Images/Image.hpp"
#include "Graphics/Vulkan/Images/ImageView.hpp"
#include "Graphics/Vulkan/Images/Sampler.hpp"
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
};
} // namespace VK

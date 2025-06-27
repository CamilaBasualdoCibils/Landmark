#include "Texture.hpp"
#include "Graphics/Vulkan/Images/ImageView.hpp"
#include "Graphics/Vulkan/Memory/Memory.hpp"

VK::Texture::Texture(const VK::TextureProperties &Properties)
    : image(Properties.ImageProp), imageView(Properties.ImageViewProp), sampler(Properties.SamplerProp)
{
}
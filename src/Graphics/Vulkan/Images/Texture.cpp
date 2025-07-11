#include "Texture.hpp"
#include "Graphics/DearImGui.hpp"
#include "Graphics/Vulkan/Images/ImageView.hpp"
#include "Graphics/Vulkan/Memory/Memory.hpp"

VK::Texture::Texture(const VK::TextureProperties &Properties)
    : image(GPURef<VK::Image>::MakeRef(Properties.ImageProp)),
      imageView(GPURef<VK::ImageView>::MakeRef(image, Properties.ImageViewProp)),
      sampler(GPURef<VK::Sampler>::MakeRef(Properties.SamplerProp))
{
}

VK::Texture::~Texture()
{
    if (ImGuiDSet)
        ImGui_ImplVulkan_RemoveTexture(ImGuiDSet);
}
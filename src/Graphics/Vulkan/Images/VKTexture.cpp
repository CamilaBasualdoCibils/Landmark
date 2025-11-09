#include "VKTexture.hpp"
//#include "Graphics/DearImGui.hpp"
#include "Graphics/Vulkan/Images/VKImageView.hpp"
#include "Graphics/Vulkan/Memory/VKMemory.hpp"

VK::Texture::Texture(const VK::TextureProperties &Properties)
    : image(Properties.ImageProp),
      imageView(image, Properties.ImageViewProp),
      sampler(Properties.SamplerProp)
{
}

VK::Texture::~Texture()
{
    if (ImGuiDSet)
        ImGui_ImplVulkan_RemoveTexture(*ImGuiDSet);
    //std::cerr << "Imgui's Image descriptor of VK::Texture leaked\n";
}
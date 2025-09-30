#pragma once
#include "Graphics/Interop/Interop.hpp"
#include "Graphics/Vulkan/Images/VKImage.hpp"
#include "Graphics/Vulkan/Images/VKImageView.hpp"
#include "Graphics/Vulkan/Images/VKSampler.hpp"
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
    mutable std::optional<VkDescriptorSet> ImGuiDSet;

  public:
    Texture(const TextureProperties &Properties);
    ~Texture();
    InteropTransaction ExportMemory() override
    {
        return image.ExportMemory();
    }

    const VK::Image& GetImage() const
    {
        return image;
    }
    VK::Image& GetImage() 
    {
        return image;
    }

    const VK::ImageView& GetImageView() const
    {
        return imageView;
    }
    VK::ImageView& GetImageView()
    {
        return imageView;
    }

    const VK::Sampler& GetSampler() const
    {
        return sampler;
    }
    VK::Sampler& GetSampler() 
    {
        return sampler;
    }

    auto GetImguiDescriptorSet() const
    {

        if (!ImGuiDSet)
            ImGuiDSet = ImGui_ImplVulkan_AddTexture((vk::Sampler)sampler, (vk::ImageView)imageView,
                                        VkImageLayout::VK_IMAGE_LAYOUT_GENERAL);
        return ImGuiDSet.value();
    }
};
} // namespace VK

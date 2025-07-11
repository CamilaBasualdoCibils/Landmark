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
    GPURef<VK::Image> image;
    GPURef<VK::ImageView> imageView;
    GPURef<VK::Sampler> sampler;
    mutable VkDescriptorSet ImGuiDSet;

  public:
    Texture(const TextureProperties &Properties);
    ~Texture();
    InteropTransaction ExportMemory() const override
    {
        return image->ExportMemory();
    }

    auto GetImage() const
    {
        return image;
    }

    auto GetImageView() const
    {
        return imageView;
    }

    auto GetSampler() const
    {
        return sampler;
    }
    auto GetImguiDescriptorSet() const
    {

        if (!ImGuiDSet)
            ImGuiDSet = ImGui_ImplVulkan_AddTexture((vk::Sampler)*sampler, (vk::ImageView)*imageView,
                                        VkImageLayout::VK_IMAGE_LAYOUT_GENERAL);
        return ImGuiDSet;
    }
};
} // namespace VK

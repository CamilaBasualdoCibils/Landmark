#include "VKImageView.hpp"
#include <Graphics/GraphicsEngine.hpp>
VK::ImageView::ImageView(VK::Image &Image, const ImageViewProperties &Properties)
{
     Handle = CreateImageView(Image,Image.GetProperties().format,Properties);
}
VK::ImageView::ImageView(vk::Image vkImage,VK::Format format, const ImageViewProperties &Properties)
{
    Handle = CreateImageView(vkImage,format,Properties);
}

VK::ImageView::~ImageView()
{
    GraphicsEngine::Get().GetMainGPU()->VK()->LogicalDevice()->GetHandle().destroy(Handle);
}

vk::ImageView VK::ImageView::CreateImageView(vk::Image image,VK::Format format, const ImageViewProperties &Properties)
{
     vk::ImageViewCreateInfo CreateInfo;

    CreateInfo.image = image;
    CreateInfo.format = (vk::Format)format;

    CreateInfo.viewType = vk::ImageViewType::e2D;
    CreateInfo.components.r = vk::ComponentSwizzle::eIdentity;
    CreateInfo.components.g = vk::ComponentSwizzle::eIdentity;
    CreateInfo.components.b = vk::ComponentSwizzle::eIdentity;
    CreateInfo.components.a = vk::ComponentSwizzle::eIdentity;
    CreateInfo.subresourceRange.aspectMask = (vk::Flags<vk::ImageAspectFlagBits>)Properties.AspectMask;
    CreateInfo.subresourceRange.baseMipLevel = 0;
    CreateInfo.subresourceRange.levelCount = 1;
    CreateInfo.subresourceRange.baseArrayLayer = 0;
    CreateInfo.subresourceRange.layerCount = 1;
    const auto CreateResult =
        GraphicsEngine::Get().GetMainGPU()->VK()->LogicalDevice()->GetHandle().createImageView(CreateInfo);
    LASSERT(CreateResult.result == vk::Result::eSuccess, "damn");
    return CreateResult.value;
}

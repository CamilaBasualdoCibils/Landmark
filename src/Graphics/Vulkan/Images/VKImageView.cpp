#include "VKImageView.hpp"
#include <Graphics/GraphicsEngine.hpp>
VK::ImageView::ImageView(std::variant<GPURef<VK::Image>, vk::Image> Image, const ImageViewProperties &Properties)
{
    vk::ImageViewCreateInfo CreateInfo;
    if (std::holds_alternative<GPURef<VK::Image>>(Image))
    {
        auto ImageRes = std::get<GPURef<VK::Image>>(Image);
        CreateInfo.image = ImageRes->GetHandle();
        CreateInfo.format = (vk::Format)ImageRes->GetProperties().format;
    }
    else
    {
        CreateInfo.image = std::get<vk::Image>(Image);
        CreateInfo.format = vk::Format::eB8G8R8A8Unorm;
    }

    CreateInfo.viewType = vk::ImageViewType::e2D;
    CreateInfo.components.r = vk::ComponentSwizzle::eIdentity;
    CreateInfo.components.g = vk::ComponentSwizzle::eIdentity;
    CreateInfo.components.b = vk::ComponentSwizzle::eIdentity;
    CreateInfo.components.a = vk::ComponentSwizzle::eIdentity;
    CreateInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
    CreateInfo.subresourceRange.baseMipLevel = 0;
    CreateInfo.subresourceRange.levelCount = 1;
    CreateInfo.subresourceRange.baseArrayLayer = 0;
    CreateInfo.subresourceRange.layerCount = 1;
    const auto CreateResult =
        GraphicsEngine::Get().GetMainGPU()->VK()->LogicalDevice()->GetHandle().createImageView(CreateInfo);
    LASSERT(CreateResult.result == vk::Result::eSuccess, "damn");
    Handle = CreateResult.value;
}
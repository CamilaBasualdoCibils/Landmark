#include "ImageView.h"
#include "Image.h"

void ImageView::Create(Image &image)
{
    if (imageView != VK_NULL_HANDLE)
    {
        GetvkDevice().destroyImageView(imageView);
    }
    vk::ImageViewCreateInfo image_view_create_info;
    image_view_create_info.image = image;
    Image::ImageProperties image_info = image.GetProperties();
    const static vk::ImageViewType nonarrayTypes[] = {vk::ImageViewType::e1D, vk::ImageViewType::e2D, vk::ImageViewType::e3D};
    const static vk::ImageViewType ArrayTypes[] = {vk::ImageViewType::e1D, vk::ImageViewType::e2D};

    const bool hasZ = image_info.dimensions.z > 1;
    const bool hasY = image_info.dimensions.y > 1;
    const bool HasLayers = image_info.layers > 1;

    image_view_create_info.viewType = HasLayers ? ArrayTypes[hasY + hasZ] : nonarrayTypes[hasY + hasZ];
    image_view_create_info.format = static_cast<vk::Format>(image_info.format);
    image_view_create_info.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
    image_view_create_info.subresourceRange.baseArrayLayer = 0;
    image_view_create_info.subresourceRange.baseMipLevel = 0;
    image_view_create_info.subresourceRange.levelCount = image_info.mipLevels;
    image_view_create_info.subresourceRange.layerCount = image_info.layers;
    

    imageView = GetvkDevice().createImageView(image_view_create_info).value;
}
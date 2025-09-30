#pragma once
#include "Types/Flags.hpp"
#include "VKImage.hpp"
#include <pch.h>
namespace VK
{
struct ImageViewProperties
{
    Flags<ImageAspect> AspectMask;
};
class ImageView
{
    vk::ImageView Handle;

  public:
    ImageView(VK::Image &Image, const ImageViewProperties &Properties);
    ImageView(vk::Image vkImage,VK::Format format, const ImageViewProperties &Properties);
    ~ImageView();
    vk::ImageView GetHandle() const
    {
        return Handle;
    }
    operator vk::ImageView() const
    {
        return GetHandle();
    }

  private:
    static vk::ImageView CreateImageView(vk::Image image,VK::Format format, const ImageViewProperties &Properties);
};
} // namespace VK
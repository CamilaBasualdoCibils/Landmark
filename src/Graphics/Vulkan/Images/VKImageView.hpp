#pragma once
#include <pch.h>
#include "VKImage.hpp"
#include "Types/Flags.hpp"
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
    ImageView(std::variant<GPURef<VK::Image>,vk::Image> Image,const ImageViewProperties& Properties);
    ~ImageView();
    vk::ImageView GetHandle() const {return Handle;}
    operator vk::ImageView() const {return GetHandle();}


};
} // namespace VK
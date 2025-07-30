#pragma once
#include <pch.h>
#include "VKImage.hpp"
namespace VK
{
    struct ImageViewProperties
    {
        
    };
class ImageView
{
    vk::ImageView Handle;
    public:
    ImageView(std::variant<GPURef<VK::Image>,vk::Image> Image,const ImageViewProperties& Properties);
    vk::ImageView GetHandle() const {return Handle;}
    operator vk::ImageView() const {return GetHandle();}


};
} // namespace VK
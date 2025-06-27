#pragma once
#include "Graphics/Interop/Interop.hpp"
#include <pch.h>
#include <vulkan/vulkan_handles.hpp>

namespace VK
{
    struct ImageViewProperties
    {

    };
class ImageView
{
    vk::ImageView Handle;
    public:
    ImageView(const ImageViewProperties& Properties){}
    vk::ImageView GetHandle() const {return Handle;}
    operator vk::ImageView() const {return GetHandle();}


};
} // namespace VK
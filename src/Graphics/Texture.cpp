#include "Texture.hpp"
#include "Graphics/OpenGL/Textures/TextureInterop.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Vulkan/Images/Texture.hpp"
#include "pch.h"

Graphics::Texture::Texture(const Graphics::TextureProperties &Properties)
{

    GL::TextureInteropProperties GLProps;
    GLProps.Dimensions = Properties.Dimensions;
    GLProps.Levels = Properties.Levels;
    LASSERT(Properties.Format.toGLFormat().has_value(), "Format conversion not specified");
    GLProps.TextureFormat = Properties.Format.toGLFormat()->first;
    GLProps.PixelFormat = Properties.Format.toGLFormat()->second;
    VK::TextureProperties VKProps;
    VKProps.ImageProp.Dimensions = Properties.Dimensions;
    LASSERT(Properties.Format.toVKFormat().has_value(), "didnt define a conversion for the format");
    VKProps.ImageProp.format = *Properties.Format.toVKFormat();
    //VKProps.ImageProp.Layers = 
    //VKProps.ImageProp.MipLevels = Properties.Levels;

    Initialize(VKProps,GLProps);
}
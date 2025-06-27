#include "Texture.hpp"
#include "Graphics/OpenGL/Textures/TextureInterop.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Vulkan/Images/Texture.hpp"

Graphics::Texture::Texture(const Graphics::TextureProperties &Properties)
{

    GL::TextureInteropProperties GLProps;
    GLProps.Dimensions = Properties.Dimensions;
    GLProps.Levels = Properties.Levels;
    GLProps.Format = *Properties.Format.toGLFormat();
    VK::TextureProperties VKProps;
    VKProps.ImageProp.Dimensions = Properties.Dimensions;
    VKProps.ImageProp.Format = *Properties.Format.toVKFormat();
    //VKProps.ImageProp.Layers = 
    //VKProps.ImageProp.MipLevels = Properties.Levels;

    Initialize(VKProps,GLProps);
}
#include "ITexture.hpp"
#include "pch.h"

GL::TextureTypes GL::ITexture::TypeFromProperties(const ITextureProperties &Properties)
{
    if (Properties.Levels <= 1)
    {
        if (Properties.Dimensions.y <= 1)
        {
            if (Properties.Dimensions.z <= 1)
            {
                return GL::TextureTypes::eTexture1D;
            }
            else
            {
                LASSERT(false, "Invalid format");
            }
        }
        else
        {
            if (Properties.Dimensions.z <= 1)
            {
                return GL::TextureTypes::eTexture2D;
            }
            else
            {
                return GL::TextureTypes::eTexture3D;
            }
        }
    }
    else
    {
        return (GL::TextureTypes)-1;
    }
}
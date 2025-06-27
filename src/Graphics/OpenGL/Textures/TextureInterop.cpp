#include "TextureInterop.hpp"
#include "Graphics/OpenGL/Textures/ITexture.hpp"
#include "Graphics/OpenGL/Textures/TextureInterop.hpp"
#include "pch.h"

GL::TextureInterop::TextureInterop(const GL::TextureInteropProperties &Properties) : ITexture(Properties)
{
    // glcreate texture called in ITexture

}
void GL::TextureInterop::ImportMemory(InteropTransaction transaction)
{
    memObject.ImportMemory(transaction);
    GLenum GLFormat = (GLenum)Format;
    const bool HasZ = Dimensions.z > 1, HasY = Dimensions.y > 1;
    if (HasY)
    {
        if (HasZ)
        {
            glTextureStorageMem3DEXT(Handle, Levels, GLFormat, Dimensions.x, Dimensions.y, Dimensions.z, memObject, 0);
        }
        else
        {
            glTextureStorageMem2DEXT(Handle, Levels, GLFormat, Dimensions.x, Dimensions.y, memObject, 0);
        }
    }
    else
    {
        LASSERT(!HasZ, "Invalid use");
        glTextureStorageMem1DEXT(Handle, Levels, GLFormat, Dimensions.x, memObject, 0);
    }
}
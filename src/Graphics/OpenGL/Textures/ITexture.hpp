#pragma once

#include <GL/gl.h>
#include <GL/glext.h>
#include <pch.h>

namespace GL
{

enum class TextureTypes : GLenum
{
    eTexture1D = GL_TEXTURE_1D,
    eTexture2D = GL_TEXTURE_2D,
    eTexture3D = GL_TEXTURE_3D,
    eTexture1DArray = GL_TEXTURE_1D_ARRAY,
    eTexture2DArray = GL_TEXTURE_2D_ARRAY,
    eTextureRectangle = GL_TEXTURE_RECTANGLE,
    eTextureCubeMap = GL_TEXTURE_CUBE_MAP,
    eTextureCubeMapArray = GL_TEXTURE_CUBE_MAP_ARRAY,
    eTextureBuffer = GL_TEXTURE_BUFFER,
    eTexture2DMultiSample = GL_TEXTURE_2D_MULTISAMPLE,
    eTexture2DMultiSampleArray = GL_TEXTURE_2D_MULTISAMPLE_ARRAY
};
enum class PixelFormats : GLenum
{
    eR = GL_RED,
    eRG = GL_RG,
    eRGB = GL_RGB,
    eRGBA = GL_RGBA,
    eBGR = GL_BGR,
    eBGRA = GL_BGRA
};
enum class TextureFormats : GLenum
{
    // Unsigned normalized formats
    eR8_UNorm = GL_R8,
    eRG8_UNorm = GL_RG8,
    eRGB8_UNorm = GL_RGB8,
    eRGBA8_UNorm = GL_RGBA8,

    // Signed normalized formats
    eR8_SNorm = GL_R8_SNORM,
    eRG8_SNorm = GL_RG8_SNORM,
    eRGB8_SNorm = GL_RGB8_SNORM,
    eRGBA8_SNorm = GL_RGBA8_SNORM,

    // Floating point formats
    eR16F = GL_R16F,
    eRG16F = GL_RG16F,
    eRGB16F = GL_RGB16F,
    eRGBA16F = GL_RGBA16F,

    eR32F = GL_R32F,
    eRG32F = GL_RG32F,
    eRGB32F = GL_RGB32F,
    eRGBA32F = GL_RGBA32F,

    // Unsigned integer formats
    eR8UI = GL_R8UI,
    eRG8UI = GL_RG8UI,
    eRGB8UI = GL_RGB8UI,
    eRGBA8UI = GL_RGBA8UI,

    eR16UI = GL_R16UI,
    eRG16UI = GL_RG16UI,
    eRGB16UI = GL_RGB16UI,
    eRGBA16UI = GL_RGBA16UI,

    eR32UI = GL_R32UI,
    eRG32UI = GL_RG32UI,
    eRGB32UI = GL_RGB32UI,
    eRGBA32UI = GL_RGBA32UI,

    // Signed integer formats
    eR8I = GL_R8I,
    eRG8I = GL_RG8I,
    eRGB8I = GL_RGB8I,
    eRGBA8I = GL_RGBA8I,

    eR16I = GL_R16I,
    eRG16I = GL_RG16I,
    eRGB16I = GL_RGB16I,
    eRGBA16I = GL_RGBA16I,

    eR32I = GL_R32I,
    eRG32I = GL_RG32I,
    eRGB32I = GL_RGB32I,
    eRGBA32I = GL_RGBA32I,

    // Packed formats
    eRGB10A2_UNorm = GL_RGB10_A2,
    eRGB10A2UI = GL_RGB10_A2UI,
    eR11G11B10F = GL_R11F_G11F_B10F,
    eRGB9_E5 = GL_RGB9_E5,
};
struct ITextureProperties
{
    TextureFormats TextureFormat;
    PixelFormats PixelFormat;
    uvec3 Dimensions;
    uint32_t Levels;
};
class ITexture
{

  protected:
    GLuint Handle;
    TextureFormats TextureFormat;
    PixelFormats PixelFormat;
    uvec3 Dimensions;
    uint32_t Levels;

  public:
    ITexture(const ITextureProperties &Properties)
    {
        TextureFormat = Properties.TextureFormat;
        PixelFormat = Properties.PixelFormat;
        Dimensions = Dimensions = max(uvec3(1), Properties.Dimensions);
        Levels = Properties.Levels;
        glCreateTextures((GLenum)TypeFromProperties(Properties), 1, &Handle);
    }
    ~ITexture()
    {
        glDeleteTextures(1, &Handle);
    }

    static TextureTypes TypeFromProperties(const ITextureProperties &Properties);
    operator GLuint() const
    {
        return Handle;
    }
    TextureFormats GetTextureFormat() const
    {
        return TextureFormat;
    }
     PixelFormats GetPixelFormat() const
    {
        return PixelFormat;
    }
};
} // namespace GL
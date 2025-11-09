#pragma once
#include "Texture.hpp"
#include "misc/Singleton.hpp"
#include <pch.h>

namespace Graphics
{
class FormatParser : public Singleton<FormatParser>
{
    public:
    enum class TextureComponents
    {
        eR,eG,eB,eA,eDepth,//Single Components
        eRG,eGR, // Double Components
        eRGB,eBGR, // triple components
        eRGBA,eABGR
    };
    enum class DataRequirements
    {
        eEqual,eAtleast
    };
    enum class DataType
    {
        eUNorm,// [0,255] -> [0.0f,1.0f]
        eSNorm,// [-127,128] -> [-1.0f,1.0f]
        eUInt, // Unsigned Integer
        eSInt, // Signed Integer
        eSRGB, // sRGB format [0,255] -> [0.0f,1.0f] gamma corrected,
        eSFloat, // Signed float [float_min,float_max] -> [float_min,float_max]
        eUFloat, // Signed float [0.0f,float_max] -> [0.0f,float_max],
        eDepth, // special case for depth
    };
    enum class FormatType
    {
        eGeneric,
        eHDR,
        eDepth,
        eCompressed,
    };
    struct FormatEntry
    {

    };
  public:
    FormatParser();
    TextureFormat FindTextureFormat(TextureComponents components,DataRequirements ComponentsRequirement,DataType dataType,FormatType formatType)
    {
        return TextureFormat::Values::eBGRA8_UNorm;
    }
};
} // namespace Graphics
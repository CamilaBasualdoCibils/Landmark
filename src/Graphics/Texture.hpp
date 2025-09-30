#pragma once
#include "Graphics/Interop/Interop.hpp"
#include "Graphics/OpenGL/Textures/GLITexture.hpp"
#include "Graphics/OpenGL/Textures/GLTextureInterop.hpp"
#include "Graphics/Vulkan/Images/VKImage.hpp"
#include "Graphics/Vulkan/Images/VKTexture.hpp"
#include "Graphics/Vulkan/VKEnums.hpp"
#include <pch.h>

namespace Graphics
{
enum class TextureLayout : std::underlying_type<VK::ImageLayouts>::type
{
    eUndefined = (size_t)VK::ImageLayouts::eUndefined,
    eColorAttachment = (size_t)VK::ImageLayouts::eColorAttachment,
    eDepthAttachment = (size_t)VK::ImageLayouts::eDepthAttachment,
    ePresentation = (size_t)VK::ImageLayouts::ePresentSrc,
    eTransferDst = (size_t)VK::ImageLayouts::eTransferDst,
    eTransferSrc = (size_t)VK::ImageLayouts::eTransferSrc,
    eShaderReadOnly = (size_t)VK::ImageLayouts::eShaderReadOnly,
};
enum class TextureUsage
{
    eColorAttachment,
    eSampled,
    eTransferSrc,
    eTransferDst
};


struct TextureFormat
{
    enum class Values
{
    eRGBA8_UNorm,  // uint8 RGBA
    eRGB8_UNorm,   // uint8 RGB
    eRGBA8_SRGB,   // uint8 RGBA (sRGB)
    eBGRA8_UNorm,  // uint8 BGRA
    eRGBA16_UNorm, // uint16 RGBA
    eR8_UNorm,     // uint8 R
    eR16_UNorm,    // uint16 R
    eRG8_UNorm,
    eRG16_UNorm,
    eDepth32_SFloat,
    eRGB32_SFloat,
    eRGB16_SFloat,
    eRGBA32_SFloat,
    eRGBA16_SFloat
};
    TextureFormat(Values v) : value(v)
    {
    }
    Values value;

    constexpr std::optional<std::pair<GL::TextureFormats, GL::PixelFormats>> toGLFormat() const
    {
        for (auto &entry : textureFormatTable)
            if (std::get<0>(entry) == value)
                return std::get<2>(entry);
        return std::nullopt;
    }
    constexpr std::optional<VK::Format> toVKFormat() const
    {
        for (auto &entry : textureFormatTable)
            if (std::get<0>(entry) == value)
                return std::get<1>(entry);
        return std::nullopt;
    }
    static inline constexpr std::tuple<Values, VK::Format, std::pair<GL::TextureFormats, GL::PixelFormats>>
        textureFormatTable[] = {
            {
                Values::eRGBA8_UNorm,
                VK::Format::eRGBA8_UNorm,
                {GL::TextureFormats::eRGBA8_UNorm, GL::PixelFormats::eRGBA},
            },

            {Values::eBGRA8_UNorm,
             VK::Format::eBGRA8_UNorm,
             {GL::TextureFormats::eRGBA8_UNorm, GL::PixelFormats::eBGRA}},

            {Values::eRGBA8_SRGB,
             VK::Format::eRGBA8_SRGB,
             {GL::TextureFormats::eSRGB8_Alpha8, GL::PixelFormats::eRGBA}},

            {Values::eDepth32_SFloat,
             VK::Format::eDepth32_SFloat,
             {GL::TextureFormats::eDepth32F, GL::PixelFormats::eDepthComponent}},

            {Values::eRGB32_SFloat,
             VK::Format::eRGB32_SFloat,
             {GL::TextureFormats::eRGB32F, GL::PixelFormats::eRGB}},

            {Values::eRGB16_SFloat,
             VK::Format::eRGB16_SFloat,
             {GL::TextureFormats::eRGB16F, GL::PixelFormats::eRGB}},

             {Values::eRGBA32_SFloat,
             VK::Format::eRGBA32_SFloat,
             {GL::TextureFormats::eRGBA32F, GL::PixelFormats::eRGBA}},

            {Values::eRGBA16_SFloat,
             VK::Format::eRGBA16_SFloat,
             {GL::TextureFormats::eRGBA16F, GL::PixelFormats::eRGBA}},

            {Values::eRGB8_UNorm,
             VK::Format::eRGB8_UNorm,
             {GL::TextureFormats::eRGB8_UNorm, GL::PixelFormats::eRGB}},

    };
};
struct TextureProperties
{
    uvec3 Dimensions;
    uint32_t Levels = 1;
    TextureFormat Format = TextureFormat::Values::eRGBA8_UNorm;
    Flags<TextureUsage> UsageFlags = {TextureUsage::eColorAttachment, TextureUsage::eSampled,
                                      TextureUsage::eTransferSrc, TextureUsage::eTransferDst};
};
class Texture : public InteropObject<VK::Texture, GL::TextureInterop, InteropTransactionType::eVK2GL>
{

  public:
    Texture(const TextureProperties &Properties);
};
} // namespace Graphics
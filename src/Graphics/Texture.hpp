#pragma once
#include "Graphics/Interop/Interop.hpp"
#include "Graphics/OpenGL/Textures/ITexture.hpp"
#include "Graphics/OpenGL/Textures/TextureInterop.hpp"
#include "Graphics/Vulkan/Images/Image.hpp"
#include "Graphics/Vulkan/Images/ImageEnums.hpp"
#include "Graphics/Vulkan/Images/Texture.hpp"
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

enum class TextureFormatValues
{
    eRGBA8_UNorm,
    eBGRA8_UNorm,
    eRGBA16_UNorm,
    eR8_UNorm,
    eR16_UNorm,
    eRG8_UNorm,
    eRG16_UNorm
};
struct TextureFormat
{
    TextureFormat(TextureFormatValues v) : value(v)
    {
    }
    TextureFormatValues value;

    constexpr std::optional<std::pair<GL::TextureFormats, GL::PixelFormats>> toGLFormat() const
    {
        for (auto &entry : textureFormatTable)
            if (std::get<0>(entry) == value)
                return std::get<2>(entry);
        return std::nullopt;
    }
    constexpr std::optional<VK::ImageFormats> toVKFormat() const
    {
        for (auto &entry : textureFormatTable)
            if (std::get<0>(entry) == value)
                return std::get<1>(entry);
        return std::nullopt;
    }
    static inline constexpr std::tuple<TextureFormatValues, VK::ImageFormats,
                                       std::pair<GL::TextureFormats, GL::PixelFormats>>
        textureFormatTable[] = {
            {
                TextureFormatValues::eRGBA8_UNorm,
                VK::ImageFormats::eRGBA8_UNorm,
                {GL::TextureFormats::eRGBA8_UNorm, GL::PixelFormats::eRGBA},
            },
            {TextureFormatValues::eBGRA8_UNorm,
             VK::ImageFormats::eBGRA8_UNorm,
             {GL::TextureFormats::eRGBA8_UNorm, GL::PixelFormats::eBGRA}}

            // Add more...
    };
};
struct TextureProperties
{
    uvec3 Dimensions;
    uint32_t Levels;
    TextureFormat Format = TextureFormatValues::eRGBA8_UNorm;
    Flags<TextureUsage> UsageFlags = {TextureUsage::eColorAttachment, TextureUsage::eSampled,
                                      TextureUsage::eTransferSrc, TextureUsage::eTransferDst};
};
class Texture : public InteropObject<VK::Texture, GL::TextureInterop, InteropTransactionType::eVK2GL>
{

  public:
    Texture(const TextureProperties &Properties);
};
} // namespace Graphics
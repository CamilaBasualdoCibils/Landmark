#pragma once

#include "Graphics/OpenGL/Commands/GLCommandManager.hpp"
#include "Graphics/Texture.hpp"
#include <pch.h>

namespace GL
{

struct BindTexture : Command
{
    GPURef<Graphics::Texture> Texture;
    GLuint Unit;
    BindTexture(GLuint _Unit, GPURef<Graphics::Texture> _Texture) : Texture(_Texture), Unit(_Unit)
    {
    }
    void Execute(void *) override
    {
        glBindTextureUnit(Unit, Texture->GL());
    }
    std::string GetName() override
    {
        return "BindTextureUnit";
    }
};
struct ClearTextureImage : Command
{
    const GPURef<Graphics::Texture> Texture;
    const std::optional<uint32_t> Level;
    const std::optional<uvec3> Offset, Size;
    const vec<4,uint8_t> ClearColor;

    ClearTextureImage(GPURef<Graphics::Texture> _Texture, vec<4,uint8_t> clearColor, std::optional<uvec3> offset = std::nullopt,
                      std::optional<uvec3> size = std::nullopt, std::optional<uint32_t> level = std::nullopt)
        : Texture(_Texture), Level(level), ClearColor(clearColor), Offset(offset), Size(size)
    {
    }
    void Execute(void *) override
    {
        if (Size.has_value() && Offset.has_value())
        {
            glClearTexSubImage(Texture->GL(), Level.value_or(0), Offset->x, Offset->y, Offset->z, Size->x, Size->y,
                               Size->z, (GLenum)Texture->GL().GetPixelFormat(), GL_UNSIGNED_BYTE, &ClearColor.x);
        }
        else
        {
            glClearTexImage(Texture->GL(), Level.value_or(0), (GLenum)Texture->GL().GetPixelFormat(), GL_UNSIGNED_BYTE,
                            &ClearColor.x);
        }
    }
    std::string GetName() override
    {
        return "BindTextureUnit";
    }
};
} // namespace GL
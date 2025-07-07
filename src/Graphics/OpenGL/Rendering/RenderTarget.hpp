#pragma once

#include "Graphics/OpenGL/Rendering/Framebuffer.hpp"
#include <pch.h>

namespace GL
{
class RenderTarget : public Framebuffer
{
    uvec2 ViewportOffset, ViewportSize;

  public:
    RenderTarget() {}
    RenderTarget &SetViewport(uvec2 Offset, uvec2 Size)
    {
        ViewportOffset = Offset;
        ViewportSize = Size;
        return *this;
    }
    void BindDraw()
    {
        Framebuffer::BindDraw();
        glViewport(ViewportOffset.x,ViewportOffset.y,ViewportSize.x,ViewportSize.y);
    }
};
} // namespace GL
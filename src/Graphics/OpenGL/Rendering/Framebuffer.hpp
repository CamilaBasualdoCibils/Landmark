#pragma once
#include "Graphics/ICommandBuffer.hpp"
#include "Graphics/OpenGL/Textures/ITexture.hpp"
#include <pch.h>

namespace GL
{
class Framebuffer
{
    bool Dirty = true;
    std::array<GPURef<GL::ITexture>, 8> ColorAttachments;
    GPURef<GL::ITexture> DepthAttachment, StencilAttachment;
    GLuint Handle;
void CheckDirty();
  public:
    Framebuffer &Attach(uint32_t Index, GPURef<GL::ITexture> Attachment);
    Framebuffer &Detach(uint32_t Index);
    Framebuffer &AttachDepth(GPURef<GL::ITexture> Attachment);
    Framebuffer &AttachStencil(GPURef<GL::ITexture> Attachment);
    Framebuffer &AttachDepthStencil(GPURef<GL::ITexture> Attachment);
    Framebuffer();
    ~Framebuffer();

    void BindReadDraw();
    void BindDraw();
    void BindRead();


};
} // namespace GL
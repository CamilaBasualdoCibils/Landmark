#pragma once
#include "Graphics/GPURef.hpp"
#include "Graphics/OpenGL/Textures/GLITexture.hpp"
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
    Framebuffer &AttachColor(uint32_t Index, GPURef<GL::ITexture> Attachment);
    Framebuffer &DetachColor(uint32_t Index);
    Framebuffer &AttachDepth(GPURef<GL::ITexture> Attachment);
    Framebuffer &AttachStencil(GPURef<GL::ITexture> Attachment);
    Framebuffer &AttachDepthStencil(GPURef<GL::ITexture> Attachment);
    Framebuffer();
    ~Framebuffer();

    void BindReadDraw();
    void BindDraw();
    void BindRead();
    static void UnBind() {glBindFramebuffer(GL_FRAMEBUFFER, 0);}


};
} // namespace GL
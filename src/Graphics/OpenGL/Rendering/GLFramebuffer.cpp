#include "GLFramebuffer.hpp"
#include "pch.h"
#include "GLRenderTarget.hpp"

GL::Framebuffer &GL::Framebuffer::AttachColor(uint32_t Index, GPURef<GL::ITexture> Attachment)
{
    LASSERT(Attachment, "Invalid Attachment");
    LASSERT(Index < 8 && Index >= 0, "Invalid Index");
    ColorAttachments[Index] = Attachment;
    Dirty = true;
    return *this;
}
GL::Framebuffer &GL::Framebuffer::DetachColor(uint32_t Index)
{
    LASSERT(Index < 8 && Index >= 0, "Invalid Index");
    if (ColorAttachments[Index])
    {
        ColorAttachments[Index] = nullptr;
        Dirty = true;
    }
    return *this;
}
GL::Framebuffer &GL::Framebuffer::AttachDepth(GPURef<GL::ITexture> Attachment)
{
    LASSERT(Attachment, "Invalid Attachment");
    DepthAttachment = Attachment;
    Dirty = true;
    return *this;
}
GL::Framebuffer &GL::Framebuffer::AttachStencil(GPURef<GL::ITexture> Attachment)
{
    LASSERT(Attachment, "Invalid Attachment");
    StencilAttachment = Attachment;
    Dirty = true;
    return *this;
}
GL::Framebuffer &GL::Framebuffer::AttachDepthStencil(GPURef<GL::ITexture> Attachment)
{
    AttachDepth(Attachment);
    AttachStencil(Attachment);
    return *this;
}
GL::Framebuffer::Framebuffer()
{
    glCreateFramebuffers(1, &Handle);
}
GL::Framebuffer::~Framebuffer()
{
    glDeleteFramebuffers(1, &Handle);
}
void GL::Framebuffer::CheckDirty()
{
    if (Dirty)
    {
        Dirty = false;
        for (int i = 0; i < ColorAttachments.size(); i++)
        {
            const auto Texture = ColorAttachments[i];
            glNamedFramebufferTexture(Handle, GL_COLOR_ATTACHMENT0 + i, Texture ? *Texture : 0, 0);
        }
        if (DepthAttachment && StencilAttachment)
        {
            if (DepthAttachment == StencilAttachment)
            {
                glNamedFramebufferTexture(Handle, GL_DEPTH_STENCIL_ATTACHMENT, DepthAttachment ? *DepthAttachment : 0,
                                          0);
            }
            else
            {
                glNamedFramebufferTexture(Handle, GL_DEPTH_ATTACHMENT, DepthAttachment ? *DepthAttachment : 0, 0);
                glNamedFramebufferTexture(Handle, GL_STENCIL_ATTACHMENT, StencilAttachment ? *StencilAttachment : 0, 0);
            }
        }
        GLenum status = glCheckNamedFramebufferStatus(Handle, GL_FRAMEBUFFER);
        LASSERT(status == GL_FRAMEBUFFER_COMPLETE, "Invalid Framebuffer");
    }
}

void GL::Framebuffer::BindRead()
{
    CheckDirty();
    glBindFramebuffer(GL_READ_FRAMEBUFFER, Handle);
}
void GL::Framebuffer::BindDraw()
{
    CheckDirty();
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, Handle);
}
void GL::Framebuffer::BindReadDraw()
{
    CheckDirty();
    BindRead();
    BindDraw();
}
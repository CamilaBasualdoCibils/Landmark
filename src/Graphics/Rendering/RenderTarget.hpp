#pragma once

#include "Graphics/Interop/Interop.hpp"
#include "Graphics/OpenGL/Rendering/RenderTarget.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Vulkan/Rendering/RenderTarget.hpp"
#include <pch.h>

namespace Graphics
{

class RenderTarget : public InteropObject<VK::RenderTarget, GL::RenderTarget, InteropTransactionType::eNone>
{

    std::array<GPURef<Graphics::Texture>, 8> ColorAttachments;
    GPURef<Graphics::Texture> DepthAttachment, StencilAttachment;
    uvec2 ViewportOffset,ViewportSize;
  public:
    RenderTarget();
    RenderTarget &AttachColor(uint32_t Index, GPURef<Graphics::Texture> Attachment)
    {
        LASSERT(Index < 8 && Index >=0, "Invalid Index");
        LASSERT(Attachment,"What");
        ColorAttachments[Index] = Attachment;
        GL().AttachColor(Index, Attachment->GLPtr());
        VK().AttachColor(Index, Attachment->VKPtr());
        return *this;
    }
    RenderTarget &DetachColor(uint32_t Index)
    {
        LASSERT(Index < 8 && Index >=0, "Invalid Index");
        LASSERT(ColorAttachments[Index],"What");
        ColorAttachments[Index] = nullptr;
        GL().DetachColor(Index);
        VK().DetachColor(Index);
        return *this;

    }
    RenderTarget& SetViewport(uvec2 Offset,uvec2 Size)
    {
        ViewportOffset = Offset;
        ViewportSize = Size;
        GL().SetViewport(Offset,Size);
        VK().SetViewport(Offset,Size);
        return *this;
    }
    RenderTarget &AttachDepth(GPURef<Graphics::Texture> Attachment);
    RenderTarget &AttachStencil(GPURef<Graphics::Texture> Attachment);
    RenderTarget &AttachDepthStencil(GPURef<Graphics::Texture> Attachment);
};
} // namespace Graphics

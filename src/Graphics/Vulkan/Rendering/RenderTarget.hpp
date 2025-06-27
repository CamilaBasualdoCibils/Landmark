#pragma once
#include "Graphics/Vulkan/Rendering/Framebuffer.hpp"
#include "Graphics/Vulkan/Rendering/RenderPass.hpp"
#include <pch.h>

namespace Graphics
{
    struct RenderTargetProperties;
}
// Render target is a smart framebuffer that creates Renderpasses as needed so it is always render ready
namespace VK
{
class RenderTarget
{
    std::optional<VK::Framebuffer> Framebuffer;
    public:
    RenderTarget(const Graphics::RenderTargetProperties& Properties);
};
} // namespace VK
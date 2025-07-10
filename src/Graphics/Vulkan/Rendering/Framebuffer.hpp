#pragma once
#include <pch.h>
#include <Graphics/GPURef.hpp>
#include "Graphics/Vulkan/Rendering/RenderPass.hpp"

namespace VK
{
struct FramebufferProperties
{
    uvec2 Dimensions;
    uint32_t LayerCount;
    GPURef<VK::RenderPass> Renderpass;
};

class Framebuffer
{
    vk::Framebuffer Handle;
public:
    Framebuffer(const FramebufferProperties& Properties);
};
} // namespace VK
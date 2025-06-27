#pragma once
#include "Graphics/ICommandBuffer.hpp"
#include <pch.h>
#include <vulkan/vulkan_enums.hpp>
#include <vulkan/vulkan_handles.hpp>
#include <vulkan/vulkan_structs.hpp>
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
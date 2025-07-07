#pragma once
#include "CommandBuffer.hpp"
#include "Graphics/ICommandBuffer.hpp"
#include "Graphics/Rendering/RenderTarget.hpp"
#include "Graphics/Vulkan/Rendering/RenderPass.hpp"
#include <pch.h>
#include <vulkan/vulkan_structs.hpp>

namespace VK
{

struct BeginRenderPass : Command
{
    std::variant<GPURef<Graphics::RenderTarget>, GPURef<VK::RenderPass>> Target;
    std::string GetName() override
    {
        return "BeginRenderPass";
    }
    void Execute(void *context) override
    {
        vk::RenderPassBeginInfo bi;
        bi.renderPass = vk::CommandBuffer *cmdBuffer = (vk::CommandBuffer *)context;
        cmdBuffer->beginRenderPass();
    }
};
} // namespace VK
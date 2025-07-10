#pragma once
#include <pch.h>
#include "CommandManager.hpp"
#include "Graphics/Rendering/RenderTarget.hpp"
#include "Graphics/Vulkan/Rendering/RenderPass.hpp"

namespace VK
{

struct BeginRenderPassCommand : Command
{
    std::variant<GPURef<Graphics::RenderTarget>, GPURef<VK::RenderPass>> Target;
    std::string GetName() override
    {
        return "BeginRenderPass";
    }
    void Execute(GPURef<VK::CommandBuffer> cmdBuffer) override
    {
        vk::RenderPassBeginInfo bi;

        // cmdBuffer->beginRenderPass();
    }
};
struct DrawCommand : Command
{
    uint32_t VertexCount, InstanceCount, FirstVertex, FirstInstance;
    DrawCommand(uint32_t VertexCount, uint32_t InstanceCount, uint32_t FirstVertex = 0, uint32_t FirstInstance = 0)
        : VertexCount(VertexCount), InstanceCount(InstanceCount), FirstVertex(FirstVertex), FirstInstance(FirstInstance)
    {
    }

    std::string GetName() override
    {
        return "Draw";
    }
    void Execute(GPURef<VK::CommandBuffer> cmdBuffer) override
    {

        cmdBuffer->GetHandle().draw(VertexCount, InstanceCount, FirstVertex, FirstInstance);
    }
};
struct BeginRenderingCommand : Command
{
    GPURef<Graphics::RenderTarget> Target;
    BeginRenderingCommand(GPURef<Graphics::RenderTarget> Target) : Target(Target)
    {
    }
    std::string GetName() override
    {
        return "BeginRendering";
    }
    void Execute(GPURef<VK::CommandBuffer> cmdBuffer) override
    {
        auto ptr = Target->VK().AsRenderingInfo();
        cmdBuffer->GetHandle().beginRendering(ptr->first);
        const uvec2 Offset =Target->GetViewportOffset(),Size = Target->GetViewportSize();
        cmdBuffer->GetHandle().setViewport(0,{vk::Viewport{Offset.x,Offset.y,Size.x,Size.y,0,1}});
    }
};
struct EndRenderingCommand : Command
{

    EndRenderingCommand()
    {
    }
    std::string GetName() override
    {
        return "EndRendering";
    }
    void Execute(GPURef<VK::CommandBuffer> cmdBuffer) override
    {

        cmdBuffer->GetHandle().endRendering();
    }
};
} // namespace VK
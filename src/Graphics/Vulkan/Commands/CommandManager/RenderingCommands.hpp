#pragma once
#include "CommandManager.hpp"
#include "Graphics/Rendering/RenderTarget.hpp"
#include "Graphics/Vulkan/Rendering/RenderPass.hpp"
#include <misc/Conversions.hpp>
#include <pch.h>
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
struct DrawIndexedCommand : Command
{
    uint32_t IndexCount, instanceCount, firstIndex, vertexOffset, firstInstance;
    DrawIndexedCommand(uint32_t IndexCount, uint32_t instanceCount = 1, uint32_t firstIndex = 0,
                       uint32_t vertexOffset = 0, uint32_t firstInstance = 0)
        : IndexCount(IndexCount), instanceCount(instanceCount), firstIndex(firstIndex), vertexOffset(vertexOffset),
          firstInstance(firstInstance)
    {
    }

    std::string GetName() override
    {
        return "DrawIndexed";
    }
    void Execute(GPURef<VK::CommandBuffer> cmdBuffer) override
    {

        cmdBuffer->GetHandle().drawIndexed(IndexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
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
        const uvec2 Offset = Target->GetViewportOffset(), Size = Target->GetViewportSize();
        cmdBuffer->GetHandle().setViewport(
            0, {vk::Viewport{(float)Offset.x, (float)Offset.y, (float)Size.x, (float)Size.y, 0.0f, 1.0f}});
        cmdBuffer->GetHandle().setScissor(0, {GlmToVkRect(Offset, Size)});
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
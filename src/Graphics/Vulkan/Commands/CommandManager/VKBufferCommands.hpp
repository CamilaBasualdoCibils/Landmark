#pragma once
#include "VKCommandManager.hpp"

#include "Graphics/Vulkan/Buffer/VKBuffer.hpp"
#include <pch.h>

namespace VK
{

struct BindVertexBuffersCommand : Command
{
    uint32_t FirstBinding;
    std::vector<GPURef<VK::Buffer>> VertexBuffers;
    std::vector<size_t> offsets;
    BindVertexBuffersCommand(uint32_t FirstBinding, std::vector<GPURef<VK::Buffer>> VertexBuffers,
                             std::vector<size_t> offsets)
        : FirstBinding(FirstBinding), VertexBuffers(VertexBuffers), offsets(offsets)
    {
    }
    std::string GetName() override
    {
        return "BindVertexBuffers";
    }
    void Execute(GPURef<VK::CommandBuffer> cmdBuffer) override
    {
        std::vector<vk::Buffer> buffers(VertexBuffers.size());
        std::transform(VertexBuffers.begin(), VertexBuffers.end(), buffers.begin(),
                       [](GPURef<VK::Buffer> b) { return (vk::Buffer)*b; });
        cmdBuffer->GetHandle().bindVertexBuffers(FirstBinding, buffers, offsets);
        // cmdBuffer->beginRenderPass();
    }
};
struct BindIndexBufferCommand : Command
{

    GPURef<VK::Buffer> IndexBuffer;
    size_t Offset;
    VK::IndexType indexType;
    BindIndexBufferCommand(GPURef<VK::Buffer> IndexBuffer, size_t Offset, VK::IndexType indexType)
        : IndexBuffer(IndexBuffer), Offset(Offset), indexType(indexType)
    {
    }
    std::string GetName() override
    {
        return "BindIndexBuffer";
    }
    void Execute(GPURef<VK::CommandBuffer> cmdBuffer) override
    {
        cmdBuffer->GetHandle().bindIndexBuffer(*IndexBuffer, Offset, (vk::IndexType)indexType);
    }
};
} // namespace VK
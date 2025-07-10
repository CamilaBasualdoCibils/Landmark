#pragma once
#include "CommandManager.hpp"

#include "Graphics/Vulkan/Buffer/Buffer.hpp"
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
        return "BeginRenderPass";
    }
    void Execute(GPURef<VK::CommandBuffer> cmdBuffer) override
    {
        vk::RenderPassBeginInfo bi;
        std::vector<vk::Buffer> buffers(VertexBuffers.size());
        std::transform(VertexBuffers.begin(), VertexBuffers.end(), buffers.begin(),
                       [](GPURef<VK::Buffer> b) { return (vk::Buffer)*b; });
        cmdBuffer->GetHandle().bindVertexBuffers(FirstBinding, buffers, offsets);
        // cmdBuffer->beginRenderPass();
    }
};

} // namespace VK
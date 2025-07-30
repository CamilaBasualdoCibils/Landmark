#include "VKBuffer.hpp"
#include "Graphics/Vulkan/Memory/VKMemory.hpp"
#include <Graphics/GraphicsEngine.hpp>

VK::Buffer::Buffer(const VK::BufferProperties &Properties)
{
    vk::BufferCreateInfo CreateInfo;
    CreateInfo.usage = Properties.UsageFlags;
    CreateInfo.size = Properties.Size;
    const auto CreateResult =
        GraphicsEngine::Get().GetMainGPU()->VK()->LogicalDevice()->GetHandle().createBuffer(CreateInfo);
    LASSERT(CreateResult.result == vk::Result::eSuccess, "man");
    Handle = CreateResult.value;
    vk::MemoryRequirements MemRequirements =
        GraphicsEngine::Get().GetMainGPU()->VK()->LogicalDevice()->GetHandle().getBufferMemoryRequirements(Handle);

    const auto MemoryType = GraphicsEngine::Get().GetMainGPU()->VK()->LogicalDevice()->FindMemoryFor(
        MemRequirements, Properties.memoryProperties);

    Memory = GPURef<VK::Memory>::MakeRef(*MemoryType, MemRequirements.size);
    const auto BindResult =
        GraphicsEngine::Get().GetMainGPU()->VK()->LogicalDevice()->GetHandle().bindBufferMemory(Handle, *Memory, 0);
    LASSERT(BindResult == vk::Result::eSuccess, "shiet");
}
VK::Buffer::~Buffer()
{
    GraphicsEngine::Get().GetMainGPU()->VK()->LogicalDevice()->GetHandle().destroyBuffer(Handle);
}
InteropTransaction VK::Buffer::ExportMemory() const
{
    return InteropTransaction(Memory->ExportMemory());
}

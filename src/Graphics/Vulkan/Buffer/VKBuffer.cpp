#include "VKBuffer.hpp"
#include "Graphics/Vulkan/Memory/VKMemory.hpp"
#include <Graphics/GraphicsEngine.hpp>

VK::Buffer::Buffer(const VK::BufferProperties &Properties)
{
    vk::ExternalMemoryBufferCreateInfo externalBufferCreateInfo = {};
    externalBufferCreateInfo.handleTypes = vk::ExternalMemoryHandleTypeFlagBits::eOpaqueFd;
    vk::BufferCreateInfo CreateInfo;
    CreateInfo.usage = Properties.UsageFlags;
    CreateInfo.size = Properties.Size;
    CreateInfo.pNext = &externalBufferCreateInfo;
    const auto CreateResult = GraphicsEngine::Get().GetMainGPU()->VK()->GetAllocator()->CreateBuffer(
        CreateInfo, Handle, Allocation);
    LASSERT(CreateResult == vk::Result::eSuccess, "shit");
}
VK::Buffer::~Buffer()
{
    GraphicsEngine::Get().GetMainGPU()->VK()->GetAllocator()->DestroyBuffer(Handle, Allocation);
}
InteropTransaction VK::Buffer::ExportMemory()
{
    LASSERT(false,"Not implemented");
    return InteropTransaction();
}

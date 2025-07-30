#include "VKCommandBuffer.hpp"
#include <Graphics/GraphicsEngine.hpp>
VK::CommandBuffer::CommandBuffer(GPURef<VK::CommandPool> Pool) : Pool(Pool)
{
    vk::CommandBufferAllocateInfo AllocInfo;
    AllocInfo.commandPool = *Pool;
    AllocInfo.commandBufferCount = 1;
    AllocInfo.level = vk::CommandBufferLevel::ePrimary;
    const auto AllocResult =
        GraphicsEngine::Get().GetMainGPU()->VK()->LogicalDevice()->GetHandle().allocateCommandBuffers(AllocInfo);
    LASSERT(AllocResult.result == vk::Result::eSuccess, "man");
    Handle = AllocResult.value.front();
}

VK::CommandBuffer::~CommandBuffer()
{
    GraphicsEngine::Get().GetMainGPU()->VK()->LogicalDevice()->GetHandle().freeCommandBuffers(*Pool, {Handle});
}

void VK::CommandBuffer::Begin()
{
    LASSERT(!Recording, "Already Begun");
    vk::CommandBufferBeginInfo BeginInfo;
    const auto BeginResult = Handle.begin(BeginInfo);
    LASSERT(BeginResult == vk::Result::eSuccess, ":(");
    Recording = true;
}

void VK::CommandBuffer::End()
{
    LASSERT(Recording, "Not begun");

    const auto EndResult = Handle.end();
    LASSERT(EndResult == vk::Result::eSuccess, ":(");
    Recording = false;
}
void VK::CommandBuffer::Reset()
{
    LASSERT(Handle.reset() == vk::Result::eSuccess,"man");
}
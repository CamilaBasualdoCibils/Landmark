#include "VKCommandPool.hpp"
#include <Graphics/GraphicsEngine.hpp>
VK::CommandPool::CommandPool(GPURef<VK::Queue> Queue)
{
    vk::CommandPoolCreateInfo CreateInfo;
    CreateInfo.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
    CreateInfo.queueFamilyIndex = Queue->GetFamily().FamilyIndex;
    const auto CreateResult = GraphicsEngine::Get().GetMainGPU()->VK()->LogicalDevice()->GetHandle().createCommandPool(CreateInfo);
    LASSERT(CreateResult.result == vk::Result::eSuccess,":(");
    Handle = CreateResult.value;
}

VK::CommandPool::~CommandPool()
{
    GraphicsEngine::Get().GetMainGPU()->VK()->LogicalDevice()->GetHandle().destroyCommandPool(Handle);
}

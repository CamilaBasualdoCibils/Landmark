#include "CommandBuffer.hpp"
#include "Graphics/GraphicsEngine.hpp"
#include "pch.h"
#include <vulkan/vulkan_enums.hpp>
#include <vulkan/vulkan_structs.hpp>

VK::CommandBuffer::CommandBuffer(){
    vk::CommandPoolCreateInfo poolCreateInfo;
    poolCreateInfo.queueFamilyIndex = 0;
    poolCreateInfo.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
    const auto CreatePoolResult =GraphicsEngine::Get().GetMainGPU()->GetVulkanDevice()->GetHandle().createCommandPool(poolCreateInfo);
    LASSERT(CreatePoolResult.result == vk::Result::eSuccess,"SHIT");
    commandPool = CreatePoolResult.value;
}

void VK::CommandBuffer::MakePartition()
{

    Partitions.emplace_back();
    Partition &P = Partitions.back();
    vk::CommandBufferAllocateInfo allocInfo;
    allocInfo.setCommandPool(commandPool);
    allocInfo.commandBufferCount = 1;
    allocInfo.level = vk::CommandBufferLevel::ePrimary;
    const auto AllocResult = GraphicsEngine::Get().GetMainGPU()->GetVulkanDevice()->GetHandle().allocateCommandBuffers(allocInfo);
    LASSERT(AllocResult.result == vk::Result::eSuccess, "SHIT");
    P.cmdBufferHandle = AllocResult.value.front();
    
}
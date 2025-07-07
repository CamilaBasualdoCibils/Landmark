#include "Graphics/Vulkan/Memory/Memory.hpp"
#include "Graphics/GraphicsEngine.hpp"
#include "pch.h"
#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan_enums.hpp>

VK::Memory::Memory(const VK::MemoryType& type,size_t size)
{
    Size = size;
    vk::ExportMemoryAllocateInfo exportInfo;
    exportInfo.handleTypes = vk::ExternalMemoryHandleTypeFlagBits::eOpaqueFd;
    vk::MemoryAllocateInfo MemAllocInfo;
    MemAllocInfo.allocationSize = size;
    MemAllocInfo.memoryTypeIndex = type.Index;
    MemAllocInfo.pNext = &exportInfo;
    const auto AllocResult = GraphicsEngine::Get().GetMainGPU()->VK()->LogicalDevice()->GetHandle().allocateMemory(MemAllocInfo);
    LASSERT(AllocResult.result == vk::Result::eSuccess,"Darn");
    handle = AllocResult.value;

    vk::MemoryGetFdInfoKHR getFdInfo;
    getFdInfo.memory = handle,
    getFdInfo.handleType = vk::ExternalMemoryHandleTypeFlagBits::eOpaqueFd;
    const auto GetMemoryResult = GraphicsEngine::Get().GetMainGPU()->VK()->LogicalDevice()->GetMemoryFdKHR(getFdInfo,&fdHandle);
    LASSERT(GetMemoryResult == VkResult::VK_SUCCESS, "maaan");

}
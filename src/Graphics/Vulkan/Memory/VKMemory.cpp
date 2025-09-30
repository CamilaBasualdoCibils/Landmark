#include "Graphics/Vulkan/Memory/VKMemory.hpp"
#include "Graphics/GraphicsEngine.hpp"
#include "pch.h"
#include "VKMemory.hpp"

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
    Handle = AllocResult.value;

    vk::MemoryGetFdInfoKHR getFdInfo;
    getFdInfo.memory = Handle,
    getFdInfo.handleType = vk::ExternalMemoryHandleTypeFlagBits::eOpaqueFd;
    const auto GetMemoryResult = GraphicsEngine::Get().GetMainGPU()->VK()->LogicalDevice()->GetMemoryFdKHR((VkMemoryGetFdInfoKHR*)&getFdInfo,&fdHandle);
    LASSERT(GetMemoryResult == VkResult::VK_SUCCESS, "maaan");

}
 VK::Memory::~Memory()
{
    GraphicsEngine::Get().GetMainGPU()->VK()->LogicalDevice()->GetHandle().freeMemory(Handle);
}
void VK::Memory::Unmap()
{
    LASSERT(MappedMemory,"Invalid Use");
    GraphicsEngine::Get().GetMainGPU()->VK()->LogicalDevice()->GetHandle().unmapMemory(Handle);
    MappedMemory = nullptr;
}
void* VK::Memory::Map()
{
    if (MappedMemory) return MappedMemory;
    const auto MapResult = GraphicsEngine::Get().GetMainGPU()->VK()->LogicalDevice()->GetHandle().mapMemory(Handle,0,Size);
    LASSERT(MapResult.result == vk::Result::eSuccess,"fuck");
    MappedMemory = MapResult.value;
    return MappedMemory;
}

#include "VKAllocation.hpp"
#include "Graphics/Vulkan/VKDevice.hpp"
#include "VKAllocator.hpp"
#include "VMAImpl.hpp"
vk::DeviceSize VK::Allocation::GetOffset() const
{
    return alloc->GetOffset();
}

InteropTransaction VK::Allocation::ExportMemory()
{
    if (!InteropTrans.has_value())
    {
        InteropTrans.emplace(InteropTransaction());
        vk::MemoryGetFdInfoKHR GetFDInfo;
        GetFDInfo.handleType = vk::ExternalMemoryHandleTypeFlagBits::eOpaqueFd;
        GetFDInfo.memory = GetDeviceMemory();
        
        device.lock()->GetMemoryFdKHR((VkMemoryGetFdInfoKHR*)&GetFDInfo,  &InteropTrans->fdHandle);
    }
    return *InteropTrans;
}
vk::DeviceMemory VK::Allocation::GetDeviceMemory() const
{
    return alloc->GetMemory();
}
void *VK::Allocation::Map()
{
return allocator->Map(*this);
}
void VK::Allocation::UnMap()
{
    allocator->UnMap(*this);
}
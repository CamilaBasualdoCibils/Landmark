#pragma once
#include "PhysicalDevice.hpp"
#include <pch.h>
#include <type_traits>

#define VKGETPROCADDRESSMEMBER(proc)                                                                                   \
    PFN_vk##proc proc##_ptr = nullptr;                                                                                 \
    template <typename... Args> auto proc(Args &&...args) -> decltype(auto)                                            \
    {                                                                                                                  \
        if (proc##_ptr == nullptr)                                                                                     \
            proc##_ptr = (PFN_vk##proc)GetProcAddress("vk" #proc);                                                     \
        return proc##_ptr((VkDevice)GetHandle(), std::forward<Args>(args)...);                                         \
    }
namespace VK
{
struct QueueSelection
{
    uint32_t FamilyIndex;
};
struct DeviceProperties
{
    std::vector<QueueSelection> Queues;
};
struct ProcAddresses
{
};
class Device : public PhysicalDevice
{
    friend PhysicalDevice;
    vk::Device DeviceHandle;

  public:
    Device(GPURef<PhysicalDevice> phyDev, const DeviceProperties &properties);
    // vk::Device* operator->() {return &DeviceHandle;}
    [[nodiscard]] auto GetHandle() const
    {
        return DeviceHandle;
    }
    [[nodiscard]] PFN_vkVoidFunction GetProcAddress(const std::string &ProcName) const
    {
        return GetHandle().getProcAddr(ProcName.c_str());
    }

    VKGETPROCADDRESSMEMBER(GetSemaphoreFdKHR);
    VKGETPROCADDRESSMEMBER(GetMemoryFdKHR);

    //
};
} // namespace VK
#include "VKAllocator.hpp"
#include "Graphics/Vulkan/VKDevice.hpp"
#include "Graphics/Vulkan/VKInstance.hpp"
#include "VKAllocation.hpp"
VK::Allocator::Allocator(GPURef<VK::Device> Device)
{
    VmaVulkanFunctions vulkanFunctions = {};
    vulkanFunctions.vkGetInstanceProcAddr = &vkGetInstanceProcAddr;
    vulkanFunctions.vkGetDeviceProcAddr = &vkGetDeviceProcAddr;

    VmaAllocatorCreateInfo allocatorCreateInfo = {};
    allocatorCreateInfo.vulkanApiVersion = VK_API_VERSION_1_4;
    allocatorCreateInfo.physicalDevice = Device->GetVkPhysicalDeviceHandle();
    allocatorCreateInfo.device = Device->GetHandle();
    allocatorCreateInfo.instance = VK::Instance::Get().GetHandle();
    allocatorCreateInfo.pVulkanFunctions = &vulkanFunctions;
    auto Result = vmaCreateAllocator(&allocatorCreateInfo, &allocator);
    LASSERT(Result == VkResult::VK_SUCCESS, "shit");
    std::cerr << "Allocator Create\n";
}

VK::Allocator::~Allocator()
{
    vmaDestroyAllocator(allocator);
    std::cerr << "Allocator Destroyed\n";
}

vk::Result VK::Allocator::CreateBuffer(const vk::BufferCreateInfo &CreateInfo, vk::Buffer &outBuffer,
                                       VK::Allocation &allocation)
{
    VmaAllocationCreateInfo allocCreateInfo = {};
    allocCreateInfo.usage = VmaMemoryUsage::VMA_MEMORY_USAGE_CPU_TO_GPU;
    allocCreateInfo.flags = 0;
    VkBuffer buf;
    VK::Allocation alloc;
    alloc.allocator = GPURef_from_this();
    vk::Result result =
        (vk::Result)vmaCreateBuffer(allocator, CreateInfo, &allocCreateInfo, &buf, &alloc.alloc, &alloc.info);
    LASSERT(result == vk::Result::eSuccess, "shit");
    outBuffer = buf;
    allocation = alloc;
    return result;
}

void VK::Allocator::DestroyBuffer(vk::Buffer buffer, VK::Allocation &allocation)
{
    vmaDestroyBuffer(allocator, buffer, allocation.alloc);
    allocation = VK::Allocation{};
}

vk::Result VK::Allocator::CreateImage(const vk::ImageCreateInfo &CreateInfo, vk::Image &outImage,
                                      VK::Allocation &allocation)
{
    VmaAllocationCreateInfo allocCreateInfo = {};
    allocCreateInfo.usage = VmaMemoryUsage::VMA_MEMORY_USAGE_AUTO;
    allocCreateInfo.flags = 0;
    VkImage image;
    VK::Allocation alloc;
    alloc.allocator = GPURef_from_this();
    vk::Result result =
        (vk::Result)vmaCreateImage(allocator, CreateInfo, &allocCreateInfo, &image, &alloc.alloc, &alloc.info);
    LASSERT(result == vk::Result::eSuccess, "shit");
    outImage = image;
    allocation = alloc;
    return result;
}

void VK::Allocator::DestroyImage(vk::Image image, VK::Allocation &allocation)
{
    vmaDestroyImage(allocator, image, allocation.alloc);
    allocation = VK::Allocation{};
}

void *VK::Allocator::Map(const Allocation &alloc)
{
    void *Data;
    const auto Result = vmaMapMemory(allocator, alloc.alloc, &Data);
    LASSERT(Result == VkResult::VK_SUCCESS, "shit");
    return Data;
}

void VK::Allocator::UnMap(const Allocation &alloc)
{
    vmaUnmapMemory(allocator, alloc.alloc);
}

/*
GPURef<VK::Allocation> VK::Allocator::Allocate(vk::MemoryRequirements memRequirements, bool exportable)
{

    const VkMemoryRequirements legacyMemRequirements = memRequirements;
    VmaAllocationCreateInfo allocCreateInfo = {};
    allocCreateInfo.usage = VMA_MEMORY_USAGE_AUTO;
    allocCreateInfo.requiredFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    // allocCreateInfo.flags = vma_allocation_create_ * exportable;  // <-- exporting
    VmaAllocation alloc;
    vmaAllocateMemory(allocator, &legacyMemRequirements, &allocCreateInfo, &alloc, nullptr);
    return GPURef<VK::Allocation>::MakeRef(alloc, GPURef_from_this());
}
*/
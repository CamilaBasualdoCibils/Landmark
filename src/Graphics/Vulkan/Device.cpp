#include "Device.hpp"
#include "Graphics/GraphicsEngine.hpp"
#include "Graphics/ICommandBuffer.hpp"
#include "Graphics/Vulkan/Queue.hpp"
#include <Graphics/Vulkan/Extension.hpp>
#include <Graphics/Vulkan/Validationlayer.hpp>
#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan_structs.hpp>
VK::Device::Device(GPURef<PhysicalDevice> phyDev, const VK::DeviceProperties &properties)
    : PhysicalDevice(phyDev->GetVkPhysicalDeviceHandle())
{
    vk::DeviceCreateInfo DeviceCreateInfo;
    std::vector<const char *> extensions;
    std::vector<const char *> validationLayers;
    extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
    extensions.push_back(VK_KHR_EXTERNAL_MEMORY_EXTENSION_NAME);
    extensions.push_back(VK_KHR_EXTERNAL_MEMORY_FD_EXTENSION_NAME);
    extensions.push_back(VK_KHR_EXTERNAL_SEMAPHORE_EXTENSION_NAME);
    extensions.push_back(VK_KHR_EXTERNAL_SEMAPHORE_FD_EXTENSION_NAME);
    extensions.push_back(VK_KHR_EXTERNAL_FENCE_EXTENSION_NAME);
    extensions.push_back(VK_KHR_EXTERNAL_FENCE_FD_EXTENSION_NAME);
    extensions.push_back(VK_KHR_IMAGELESS_FRAMEBUFFER_EXTENSION_NAME);
    // extensions.push_back(VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME);
    validationLayers.push_back("VK_LAYER_KHRONOS_validation");
    vk::PhysicalDeviceFeatures DeviceFeatures;
    DeviceFeatures.geometryShader = true;
    DeviceFeatures.multiDrawIndirect = true;
    DeviceFeatures.samplerAnisotropy = true;
    DeviceFeatures.sampleRateShading = true;
    // DeviceFeatures.frame
    vk::PhysicalDeviceTimelineSemaphoreFeatures timelineFeatures = {};
    timelineFeatures.timelineSemaphore = VK_TRUE;
    DeviceCreateInfo.pNext = &timelineFeatures;
    vk::PhysicalDeviceImagelessFramebufferFeatures ImageLessFramebuffersFeatures;
    ImageLessFramebuffersFeatures.imagelessFramebuffer = true;
    timelineFeatures.setPNext(ImageLessFramebuffersFeatures);
    vk::PhysicalDeviceDynamicRenderingFeaturesKHR dynamicRenderingFeatures{};
dynamicRenderingFeatures.dynamicRendering = VK_TRUE;
ImageLessFramebuffersFeatures.setPNext(&dynamicRenderingFeatures);
    DeviceCreateInfo.setPEnabledExtensionNames(extensions);
    DeviceCreateInfo.setPEnabledLayerNames(validationLayers);
    DeviceCreateInfo.setPEnabledFeatures(&DeviceFeatures);

    
    vk::DeviceQueueCreateInfo queueGraphics, queueCompute, queuePresent;
    queueGraphics.queueCount = 1;
    queueGraphics.queueFamilyIndex = 0;
    DeviceCreateInfo.pQueueCreateInfos = &queueGraphics;
    float p = 1.0f;

    queueGraphics.pQueuePriorities = &p;
    DeviceCreateInfo.queueCreateInfoCount = 1;
    const auto CreateDeviceResult = GetVkPhysicalDeviceHandle().createDevice(DeviceCreateInfo);
    LASSERT(CreateDeviceResult.result == vk::Result::eSuccess, "Unable to create Vulkan device");
    DeviceHandle = CreateDeviceResult.value;
}

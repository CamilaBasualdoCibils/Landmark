#include "Instance.hpp"
#include <IO/GLFW/GLFW.hpp>
#include <algorithm>
#include <misc/Versions.h>
#include <vulkan/vulkan_enums.hpp>
#include <vulkan/vulkan_handles.hpp>
#include <vulkan/vulkan_structs.hpp>
#include <vulkan/vulkan_to_string.hpp>
namespace VK
{
VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(vk::DebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                             vk::DebugUtilsMessageTypeFlagsEXT messageType,
                                             const vk::DebugUtilsMessengerCallbackDataEXT *pCallbackData,
                                             void * /*pUserData*/)
{
    std::cerr << "Validation Layer: " << pCallbackData->pMessage << std::endl;
    return VK_FALSE;
}

Instance::Instance(const InstanceProperties &properties)
{
    if (properties.bIncludeGLFWExtensions)
    {
        const auto &RequiredExtensions = GLFW::Get().GetRequiredVKExtensions();
        EnabledExtensions.insert(EnabledExtensions.end(), RequiredExtensions.begin(), RequiredExtensions.end());
        std::cerr << "GLFW required extensions:\n";
        for (const auto &ext : RequiredExtensions)
            std::cerr << ext << std::endl;
    }
    // https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VK_KHR_portability_enumeration.html
    //EnabledExtensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);

    EnabledExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    // EnabledValidationLayers.push_back("VK_LAYER_LUNARG_API_DUMP");
    // EnabledValidationLayers.push_back("VK_LAYER_LUNARG_screenshot");
    // EnabledValidationLayers.push_back("VK_LAYER_KHRONOS_profiles");
    //// EnabledValidationLayers.push_back("VK_LAYER_LUNARG_monitor");
    // EnabledValidationLayers.push_back("VK_LAYER_KHRONOS_synchronization2");
    // EnabledValidationLayers.push_back("VK_LAYER_KHRONOS_validation");
    // EnabledValidationLayers.push_back("VK_LAYER_LUNARG_crash_diagnostic");
    // EnabledValidationLayers.push_back("VK_LAYER_KHRONOS_shader_object");

    vk::ApplicationInfo app_info{};
    app_info.apiVersion = vk::makeApiVersion(properties.VulkanVersion.variant, properties.VulkanVersion.major,
                                             properties.VulkanVersion.minor, properties.VulkanVersion.patch);

    vk::InstanceCreateInfo instance_create_info{};
    instance_create_info.pApplicationInfo = &app_info;
    instance_create_info.flags = vk::InstanceCreateFlagBits::eEnumeratePortabilityKHR;
    std::vector<const char *> extension_names(EnabledExtensions.size());
    std::transform(EnabledExtensions.begin(), EnabledExtensions.end(), extension_names.begin(),
                   [](const std::string &str) { return str.c_str(); });
    instance_create_info.setPEnabledExtensionNames(extension_names);

    std::vector<const char *> validation_layer_names(EnabledValidationLayers.size());
    std::transform(EnabledValidationLayers.begin(), EnabledValidationLayers.end(), validation_layer_names.begin(),
                   [](const std::string &str) { return str.c_str(); });
    instance_create_info.setPEnabledLayerNames(validation_layer_names);

    vk::ResultValue<vk::Instance> result = vk::createInstance(instance_create_info);
    Handle = result.value;
    if (result.result != vk::Result::eSuccess)
        std::cerr << vk::to_string(result.result) << std::endl;
    if (result.result != vk::Result::eSuccess)
    {
        std::cerr << "Unable to create Vulkan Instance. " << vk::to_string(result.result) << std::endl;

        const auto &availableExt = vk::enumerateInstanceExtensionProperties().value;
        for (const auto &NeedExt : EnabledExtensions)
        {
            auto it = std::find_if(availableExt.begin(), availableExt.end(), [&](const vk::ExtensionProperties &avail) {
                return NeedExt == avail.extensionName;
            });

            if (it == availableExt.end())
            {
                std::cerr << "Missing Extensions: " << NeedExt << std::endl;
            }
        }
    }
    else
        std::cout << "Vulkan Instance Init\n";

    LASSERT(result.result == vk::Result::eSuccess, "Unable to create Vulkan instance");
    dldi = {Handle, vkGetInstanceProcAddr};
    for (const auto &device : Handle.enumeratePhysicalDevices().value)
    {
        PhysicalDevices.emplace_back(std::make_shared<VK::PhysicalDevice>(device));
    }
    vk::DebugUtilsMessengerCreateInfoEXT debug_create_info{};
    debug_create_info.messageSeverity =
        vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo |
        vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError;
    debug_create_info.messageType = vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
                                    vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
                                    vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance;
    debug_create_info.pfnUserCallback = DebugCallback;
    const auto DebugMessengerResult = Handle.createDebugUtilsMessengerEXT(debug_create_info, nullptr, dldi);
    LASSERT(DebugMessengerResult.result == vk::Result::eSuccess, "MAn");
    DebugMessenger = DebugMessengerResult.value;
}

Instance::~Instance()
{
}

} // namespace VK

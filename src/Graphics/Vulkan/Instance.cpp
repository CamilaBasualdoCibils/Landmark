#include "Instance.hpp"
#include <misc/Versions.h>
#include <IO/GLFW/GLFW.hpp>
namespace VK
{
    Instance::Instance(const InstanceProperties &properties)
    {
        if (properties.bIncludeGLFWExtensions)
        {
            const auto &RequiredExtensions = GLFW::Get().GetRequiredVKExtensions();
            EnabledExtensions.insert(EnabledExtensions.end(), RequiredExtensions.begin(), RequiredExtensions.end());
        }
        // https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VK_KHR_portability_enumeration.html
        EnabledExtensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);

        EnabledExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        // EnabledValidationLayers.push_back("VK_LAYER_LUNARG_API_DUMP");
        // EnabledValidationLayers.push_back("VK_LAYER_LUNARG_screenshot");
        EnabledValidationLayers.push_back("VK_LAYER_KHRONOS_profiles");
        // EnabledValidationLayers.push_back("VK_LAYER_LUNARG_monitor");
        EnabledValidationLayers.push_back("VK_LAYER_KHRONOS_synchronization2");
        EnabledValidationLayers.push_back("VK_LAYER_KHRONOS_validation");
        EnabledValidationLayers.push_back("VK_LAYER_LUNARG_crash_diagnostic");
        EnabledValidationLayers.push_back("VK_LAYER_KHRONOS_shader_object");

        vk::ApplicationInfo app_info{};
        app_info.apiVersion = vk::makeApiVersion(properties.VulkanVersion.variant,
                                                 properties.VulkanVersion.major,
                                                 properties.VulkanVersion.minor,
                                                 properties.VulkanVersion.patch);

        vk::InstanceCreateInfo instance_create_info{};
        instance_create_info.pApplicationInfo = &app_info;
        instance_create_info.flags = vk::InstanceCreateFlagBits::eEnumeratePortabilityKHR;
        std::vector<const char *> extension_names(EnabledExtensions.size());
        std::transform(EnabledExtensions.begin(), EnabledExtensions.end(), extension_names.begin(), [](const std::string &str)
                       { return str.c_str(); });
        instance_create_info.setPEnabledExtensionNames(extension_names);

        std::vector<const char *> validation_layer_names(EnabledValidationLayers.size());
        std::transform(EnabledValidationLayers.begin(), EnabledValidationLayers.end(), validation_layer_names.begin(), [](const std::string &str)
                       { return str.c_str(); });
        instance_create_info.setPEnabledLayerNames(validation_layer_names);

        vk::ResultValue<vk::Instance> result = vk::createInstance(instance_create_info);
        Handle = result.value;
        LASSERT(result.result == vk::Result::eSuccess, "Unable to create Vulkan instance");
        if (result.result != vk::Result::eSuccess)
            std::cerr << "Unable to create Vulkan Instance. " << vk::to_string(result.result) << std::endl;
        else
            std::cout << "Vulkan Instance Init\n";

        for (const auto &device : Handle.enumeratePhysicalDevices().value)
        {
            PhysicalDevices.emplace_back(std::make_shared<VK::PhysicalDevice>(device));
        }
    }

    Instance::~Instance()
    {
    }

}

#include "Vulkan_Instance.h"
#include "Core/Window.h"
VKAPI_ATTR VkBool32 VKAPI_CALL DebugMessengerCallback(vk::DebugUtilsMessageSeverityFlagBitsEXT severity,
                                                      vk::DebugUtilsMessageTypeFlagsEXT messageType,
                                                      const vk::DebugUtilsMessengerCallbackDataEXT *pCallbackData,
                                                      void *)
{
    static Log log = Log("Vulkan-Validation");
    switch (severity)
    {

    case vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose:
    case vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo:
        log.Debug(pCallbackData->pMessage);
        break;
    case vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning:
        log.Warning(pCallbackData->pMessage);
        break;
    case vk::DebugUtilsMessageSeverityFlagBitsEXT::eError:
        log.Error(pCallbackData->pMessage);
        break;
    }
    return true;
}
Vulkan_Instance::Vulkan_Instance(const VulkanInstanceProperties &_p) : properties(_p)
{
    CreateInstance();
}

Vulkan_Instance::~Vulkan_Instance()
{
    // vk_instance.destroyDebugUtilsMessengerEXT(debugMessenger);

    vk_instance.destroy();
}

std::vector<PhysicalDevice> Vulkan_Instance::EnumerateDevices(LWindow *window)
{
    LASSERT(vk_instance != 0, "Instance not yet initialized");
    std::vector<PhysicalDevice> physical_devices;
    vk::SurfaceKHR surf = window->GetorMakeSurface(vk_instance);
    // const auto device_groups = vk_instance.enumeratePhysicalDeviceGroups().value;
    const auto enumrated_devices = vk_instance.enumeratePhysicalDevices().value;
    for (const auto &phy : enumrated_devices)
        physical_devices.emplace_back(phy, window ? &surf : nullptr);
    return physical_devices;
}

void Vulkan_Instance::CreateInstance()
{
    GLFW_Instance::GetInstance();

    if (properties.include_GLFW_required_extensions)
    {
        uint32_t glfwExtensionCount = 0;
        Extension *glfwExtensions;

        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        for (int i = 0; i < glfwExtensionCount; i++)
            enabled_extensions.push_back(glfwExtensions[i]);
    }

    // https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VK_KHR_portability_enumeration.html
    enabled_extensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);

    if (properties.validation_enable)
    {
        enabled_extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        if (properties.khronos_validation)
            enabled_validation_layers.push_back("VK_LAYER_KHRONOS_validation");
    }
    vk::ApplicationInfo app_info{};
    app_info.pApplicationName = properties.application_name.c_str();
    app_info.applicationVersion =
        vk::makeApiVersion(properties.application_version.variant, properties.application_version.major,
                           properties.application_version.minor, properties.application_version.patch);
    app_info.pEngineName = properties.engine_name.c_str();
    app_info.engineVersion = vk::makeApiVersion(properties.engine_version.variant, properties.engine_version.major,
                                                properties.engine_version.minor, properties.engine_version.patch);
    app_info.apiVersion = vk::makeApiVersion(properties.vulkan_version.variant, properties.vulkan_version.major,
                                             properties.vulkan_version.minor, properties.vulkan_version.patch);

    vk::InstanceCreateInfo instance_create_info{};
    instance_create_info.pApplicationInfo = &app_info;
    instance_create_info.flags |= vk::InstanceCreateFlagBits::eEnumeratePortabilityKHR;
    instance_create_info.enabledExtensionCount = enabled_extensions.size();
    instance_create_info.ppEnabledExtensionNames = enabled_extensions.data();
    instance_create_info.enabledLayerCount = enabled_validation_layers.size();
    instance_create_info.ppEnabledLayerNames = enabled_validation_layers.data();

    auto result = vk::createInstance(instance_create_info);

    if (result.result != vk::Result::eSuccess)
    {
        log.Critical("Failed to create Vulkan Instance: Error: " + vk::to_string(result.result));
        switch (result.result)
        {
        case vk::Result::eErrorExtensionNotPresent: {
            const auto availExt = vk::enumerateInstanceExtensionProperties().value;
            std::set<const char *> aExt;
            for (const auto &a : availExt)
                aExt.insert(a.extensionName.data());
            for (const auto &ext : enabled_extensions)
                if (!aExt.contains(ext))
                    log.Critical("Extension " + std::string(ext) + " not available in this system");
        }
        break;
        case vk::Result::eErrorLayerNotPresent: {
            const auto availLay = vk::enumerateInstanceLayerProperties().value;
            std::set<const char *> aLay;
            for (const auto &a : availLay)
                aLay.insert(a.layerName.data());
            for (const auto &lay : enabled_validation_layers)
                if (!aLay.contains(lay))
                    log.Critical("Layer " + std::string(lay) + " not available in this system");
        }
        break;

        default: {
            log.Critical("Error type not done this shits. my fault lol");
            break;
        }
        }
        throw std::runtime_error("Failed to create Vulkan Instance");
    }
    vk_instance = result.value;
    log.Debug("===== Vulkan Instance  ===== \nExtensions:");
    for (auto &ext : enabled_extensions)
        log.Debug(std::string("	") + ext);
    log.Debug("Validation Layers:");
    for (auto layer : enabled_validation_layers)
        log.Debug(std::string("	") + layer);
    log.Debug("============================");

    vk::DebugUtilsMessengerCreateInfoEXT debug_create_info{};
    debug_create_info.messageSeverity =
        vk::DebugUtilsMessageSeverityFlagBitsEXT::eError | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
        vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo | vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose;
    debug_create_info.messageType =
        // vk::DebugUtilsMessageTypeFlagBitsEXT::eDeviceAddressBinding|
        // vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral|
        vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation;
    debug_create_info.pfnUserCallback = &DebugMessengerCallback;
    auto createDebugUtilsMessengerEXT =
        reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vk_instance.getProcAddr("vkCreateDebugUtilsMessengerEXT"));
    createDebugUtilsMessengerEXT((VkInstance)vk_instance,
                                 reinterpret_cast<VkDebugUtilsMessengerCreateInfoEXT *>(&debug_create_info), nullptr,
                                 reinterpret_cast<VkDebugUtilsMessengerEXT *>(&debugMessenger));
}

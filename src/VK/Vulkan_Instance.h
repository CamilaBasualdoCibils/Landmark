#pragma once
#include <pch.h>
#include "Debug/Debug.h"
#include "VK/Devices/LogicalDevice.h"
#include "Shared_Structs.h"
class Window;
class Vulkan_Instance {
    public:
    struct VulkanInstanceProperties {
        Version4 vulkan_version;
        bool validation_enable = true;
        bool khronos_validation = true;
        bool include_GLFW_required_extensions = true; 
        std::string application_name = "Default App Name";
        Version4 application_version;
        std::string engine_name = "Default Engine Name";       
        Version4 engine_version;

    };
    using Extension = const char*;
    using Validation_Layer = const char*;
    private:
    VulkanInstanceProperties properties;

    vk::Instance vk_instance;

    vk::DebugUtilsMessengerEXT debugMessenger;
    Log log = Log("Vulkan");
    std::vector<Extension> enabled_extensions;
    std::vector<Validation_Layer> enabled_validation_layers;
    public:
    Vulkan_Instance(const VulkanInstanceProperties& _p);
    ~Vulkan_Instance();
    std::vector<PhysicalDevice> EnumerateDevices(Window* window = nullptr);
    private:
    void CreateInstance();
};
#include "Instance.hpp"
#include "Libs/GLFW.hpp"
Vulkan::Instance::Instance(const Settings &_settings) : settings(_settings) {
  CreateInstance();
}

void Vulkan::Instance::CreateInstance() {
  std::vector<const char *>
      instance_extensions =
          {
              VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
              VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME,
          },
      instance_layers = {"VK_LAYER_KHRONOS_validation"};
  GLFW::Get().GetRequiredVulkanExtensions(instance_extensions);
  vk::ApplicationInfo app_info{};
  app_info.apiVersion = vk::makeApiVersion(0, 1, 4, 0);

  vk::InstanceCreateInfo create_info;
  create_info.setPApplicationInfo(&app_info);
  create_info.flags = vk::InstanceCreateFlagBits::eEnumeratePortabilityKHR;
  create_info.setPEnabledExtensionNames(instance_extensions);
  create_info.setPEnabledLayerNames(instance_layers);

  _instance = vk::createInstance(create_info);

  std::cout << "Vulkan Instance created:" << std::endl;
  std::cout << "    Extensions:" << std::endl;
  for (const auto &ext : instance_extensions)
    std::cout << "        " << ext << std::endl;
  std::cout << "    Layers:" << std::endl;
  for (const auto &layer : instance_layers)
    std::cout << "        " << layer << std::endl;
}

void Vulkan::Instance::CreateDebugMessenger()
{
  
}
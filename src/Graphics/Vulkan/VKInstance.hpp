#include <Graphics/Vulkan/VKExtension.hpp>
#include <Graphics/Vulkan/VKPhysicalDevice.hpp>
#include <Graphics/Vulkan/VKValidationlayer.hpp>
#include <misc/Singleton.hpp>
#include <misc/Versions.h>
#include <pch.h>
namespace VK
{
struct InstanceProperties
{
    Version4 VulkanVersion = {1, 4};
    bool bIncludeGLFWExtensions = true;
};
class Instance : public Singleton<Instance, InstanceProperties>
{
  public:
    Instance(const InstanceProperties &);
    ~Instance();
    const std::vector<std::shared_ptr<VK::PhysicalDevice>> &EnumrateDevices() const
    {
        return PhysicalDevices;
    }
    operator vk::Instance() const
    {
        return Handle;
    }
    operator vk::Instance()
    {
        return Handle;
    }
    vk::detail::DispatchLoaderDynamic GetDynamicLoader() const
    {
        return dldi;
    }

  private:
    vk::Instance Handle;
    vk::DebugUtilsMessengerEXT DebugMessenger;
    vk::detail::DispatchLoaderDynamic dldi;
    std::vector<std::shared_ptr<VK::PhysicalDevice>> PhysicalDevices;
    std::vector<Extension> EnabledExtensions;
    std::vector<ValidationLayer> EnabledValidationLayers;
    /* data */
};

} // namespace VK
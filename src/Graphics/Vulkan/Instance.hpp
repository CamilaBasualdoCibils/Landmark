#include <pch.h>
#include <misc/Versions.h>
#include <misc/Singleton.hpp>
#include <Graphics/Vulkan/Extension.hpp>
#include <Graphics/Vulkan/Validationlayer.hpp>
#include <Graphics/Vulkan/PhysicalDevice.hpp>
namespace VK
{
    struct InstanceProperties
    {
        Version4 VulkanVersion = {1,4};
        bool bIncludeGLFWExtensions = true;
    };
    class Instance : public Singleton<Instance, InstanceProperties>
    {
    public:
        Instance(const InstanceProperties &);
        ~Instance();
        const std::vector<std::shared_ptr<VK::PhysicalDevice>>& EnumrateDevices() const {return PhysicalDevices;}
        operator vk::Instance() const {return Handle;}
        operator vk::Instance() {return Handle;}
    private:
        vk::Instance Handle;
        vk::DebugUtilsMessengerEXT DebugMessenger;
        vk::detail::DispatchLoaderDynamic dldi;
        std::vector<std::shared_ptr<VK::PhysicalDevice>> PhysicalDevices;
        std::vector<Extension> EnabledExtensions;
        std::vector<ValidationLayer> EnabledValidationLayers;
        /* data */
        
    };

}
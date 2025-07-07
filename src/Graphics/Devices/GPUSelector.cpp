#include "GPUSelector.hpp"
#include <Graphics/Vulkan/Instance.hpp>
#include <Graphics/Vulkan/PhysicalDevice.hpp>
#include <Graphics/OpenGL/EGL.hpp>
GPUSelector::GPUSelector()
{
    auto EGLDevices = GL::EGL::Get().EnumerateDevices();
    const auto &VulkanDevices = VK::Instance::Get().EnumrateDevices();

    for (int i = 0; i < VulkanDevices.size(); i++)
    {
        const auto &VKDevice = VulkanDevices[i];

        for (int j = 0; j < EGLDevices.size(); j++)
        {
            const auto &EGLDevice = EGLDevices[j];

            if (!std::memcmp(VKDevice->GetDeviceUUID().data(), EGLDevice->DeviceUUID.data(), 16))
            {
                AvailableDevices.emplace_back(std::make_shared<GPUPhysicalDevice>(VKDevice, EGLDevice));
                // EGLDevices.erase(EGLDevices.begin() + j);
                // VulkanDevices.erase(VulkanDevices.begin()+ i);
                // j--;i--;
            }
        }
    }
    std::cout << "Available Devices:\n";
    for (const auto &AvailableDevice : AvailableDevices)
    {
        static int i = 0;
        std::cout << "  [" << i << "] " << AvailableDevice->GetName() << std::endl;
        i++;
    }
}

std::vector<std::shared_ptr<GPUPhysicalDevice>> GPUSelector::FilterDevices(const GPUSelectorFilter &filter)
{
    return std::vector<std::shared_ptr<GPUPhysicalDevice>>();
}

const std::vector<std::shared_ptr<GPUPhysicalDevice>> &GPUSelector::AllDevices()
{
    return AvailableDevices;
}

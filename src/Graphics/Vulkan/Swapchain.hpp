#pragma once
#include "Graphics/ICommandBuffer.hpp"
#include "Graphics/Synchronization.hpp"
#include "Graphics/Vulkan/Images/ImageView.hpp"
#include "Graphics/Vulkan/Images/Sampler.hpp"
#include <pch.h>
#include <vulkan/vulkan_handles.hpp>

namespace VK
{
struct SwapchainImage
{
    vk::Image imageRaw; // RAW VK IMAGE since OS owns them
    std::optional<VK::ImageView> imageView;
    std::optional<VK::Sampler> sampler;
};

struct SwapchainProperties
{
    vk::SurfaceKHR surface;
};
class Swapchain
{
    uint32_t ImageCount;
    vk::SwapchainKHR vkSwapchainHandle;
    std::vector<SwapchainImage> Images;

    uint32_t CurrentImageIndex = 0;
public:
    Swapchain(vk::SurfaceKHR surface);
    vk::SwapchainKHR GetHandle() const {return vkSwapchainHandle;}
    operator vk::SwapchainKHR() const {return vkSwapchainHandle;}
    void AcquireNextSwapchainImage(GPURef<Graphics::Semaphore> onCompleteSemaphore);
};
} // namespace VK
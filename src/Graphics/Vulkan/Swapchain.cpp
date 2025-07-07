#include "Swapchain.hpp"
#include "Graphics/GraphicsEngine.hpp"
#include "misc/Conversions.hpp"
#include "pch.h"
#include <cstdint>
#include <vulkan/vulkan_enums.hpp>

VK::Swapchain::Swapchain(vk::SurfaceKHR surface)
{

    const auto SurfaceCapabilities =
        GraphicsEngine::Get().GetMainGPU()->VK()->LogicalDevice()->GetSurfaceCapabilities(surface);
    ImageCount = std::min(SurfaceCapabilities.minImageCount + 1,
                          SurfaceCapabilities.maxImageCount != 0 ? SurfaceCapabilities.maxImageCount : UINT32_MAX);

    uvec2 CurrentExtent = VkToGlm(SurfaceCapabilities.currentExtent),
          MaxExtent = VkToGlm(SurfaceCapabilities.maxImageExtent),
          MinExtent = VkToGlm(SurfaceCapabilities.minImageExtent);
    CurrentExtent = clamp(CurrentExtent, MinExtent, MaxExtent);
    const auto formats =
        GraphicsEngine::Get().GetMainGPU()->VK()->LogicalDevice()->GetVkPhysicalDeviceHandle().getSurfaceFormatsKHR(
            surface);
    vk::SwapchainCreateInfoKHR CreateInfo;
    CreateInfo.clipped = true;
    CreateInfo.imageArrayLayers = 1;
    CreateInfo.imageColorSpace = vk::ColorSpaceKHR::eSrgbNonlinear;
    CreateInfo.imageFormat = vk::Format::eB8G8R8A8Srgb;
    CreateInfo.presentMode = vk::PresentModeKHR::eImmediate;
    CreateInfo.imageExtent = GlmToVk(CurrentExtent);
    CreateInfo.minImageCount = ImageCount;
    CreateInfo.imageSharingMode = vk::SharingMode::eExclusive;
    CreateInfo.preTransform = vk::SurfaceTransformFlagBitsKHR::eIdentity;
    CreateInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
    CreateInfo.surface = surface;
    CreateInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransferSrc |
                            vk::ImageUsageFlagBits::eTransferDst;

    const auto CreateResult =
        GraphicsEngine::Get().GetMainGPU()->VK()->LogicalDevice()->GetHandle().createSwapchainKHR(CreateInfo);
    LASSERT(CreateResult.result == vk::Result::eSuccess, "SHIT");

    vkSwapchainHandle = CreateResult.value;
    const auto &Retrievedimages = GraphicsEngine::Get()
                                      .GetMainGPU()
                                      ->VK()
                                      ->LogicalDevice()
                                      ->GetHandle()
                                      .getSwapchainImagesKHR(vkSwapchainHandle)
                                      .value;
    Images.reserve(Retrievedimages.size());
    for (const auto &vkimage : Retrievedimages)
    {
        SwapchainImage Image;
        Image.imageRaw = vkimage;
        Images.push_back(Image);
        // Image.imageView.emplace()
    }
}
void VK::Swapchain::AcquireNextSwapchainImage(GPURef<Graphics::Semaphore> OnCompleteSemaphore)
{
    const auto AcquireResult =
        GraphicsEngine::Get().GetMainGPU()->VK()->LogicalDevice()->GetHandle().acquireNextImageKHR(
            GetHandle(), UINT64_MAX, OnCompleteSemaphore ? OnCompleteSemaphore->VK().GetHandle() : nullptr, nullptr);
    LASSERT(AcquireResult.result == vk::Result::eSuccess, "Maaaan");
    CurrentImageIndex = AcquireResult.value;
}
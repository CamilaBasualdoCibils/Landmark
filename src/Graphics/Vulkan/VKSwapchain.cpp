#include "VKSwapchain.hpp"
#include "Graphics/GraphicsEngine.hpp"
#include "misc/Conversions.hpp"
#include "pch.h"

VK::Swapchain::Swapchain(vk::SurfaceKHR surface)
{
    vkSwapchainHandle = CreateSwapchain(surface, std::nullopt, ImageFormat, ImageCount, CurrentExtent, Images);
}
VK::Swapchain::~Swapchain()
{
    GraphicsEngine::Get().GetMainGPU()->VK()->LogicalDevice()->GetHandle().destroy(vkSwapchainHandle);
}
void VK::Swapchain::AcquireNextSwapchainImage(GPURef<Graphics::Semaphore> OnCompleteSemaphore)
{
    const auto AcquireResult =
        GraphicsEngine::Get().GetMainGPU()->VK()->LogicalDevice()->GetHandle().acquireNextImageKHR(
            GetHandle(), UINT64_MAX, OnCompleteSemaphore ? OnCompleteSemaphore->VK().GetHandle() : nullptr, nullptr);
    CurrentImageIndex = AcquireResult.value;
    OutOfDate =
        (AcquireResult.result == vk::Result::eErrorOutOfDateKHR || AcquireResult.result == vk::Result::eSuboptimalKHR);
}

[[nodiscard]] std::pair<GPURef<Graphics::Semaphore>, VK::SwapchainImage> VK::Swapchain::AcquireNextSwapchainImage()
{
    GPURef<Graphics::Semaphore> semaphore = GPURef<Graphics::Semaphore>::MakeRef();
    AcquireNextSwapchainImage(semaphore);
    return {semaphore, Images[CurrentImageIndex]};
}

void VK::Swapchain::Present(GPURef<VK::Queue> queue, GPURef<Graphics::Semaphore> WaitSemaphore)
{
    vk::Semaphore SemaphoreHandle = WaitSemaphore->VK().GetHandle();
    vk::PresentInfoKHR PresentInfo{};
    vk::Result PresentResult;
    PresentInfo.pImageIndices = &CurrentImageIndex;
    PresentInfo.pSwapchains = &vkSwapchainHandle;
    PresentInfo.swapchainCount = 1;
    PresentInfo.pWaitSemaphores = &SemaphoreHandle;
    PresentInfo.waitSemaphoreCount = 1;
    PresentInfo.pResults = &PresentResult;

    const auto PresentResultOverall = queue->GetHandle().presentKHR(PresentInfo);
    OutOfDate =
        (PresentResultOverall == vk::Result::eErrorOutOfDateKHR || PresentResultOverall == vk::Result::eSuboptimalKHR);
}

void VK::Swapchain::Recreate(vk::SurfaceKHR surface)
{
    vk::SwapchainKHR newHandle =
        CreateSwapchain(surface, vkSwapchainHandle, ImageFormat, ImageCount, CurrentExtent, Images);
    GraphicsEngine::Get().GetMainGPU()->VK()->LogicalDevice()->GetHandle().destroy(vkSwapchainHandle);
    vkSwapchainHandle = newHandle;
    CurrentImageIndex = 0;
    OutOfDate = false;
}

vk::SwapchainKHR VK::Swapchain::CreateSwapchain(vk::SurfaceKHR surface, std::optional<vk::SwapchainKHR> OldSwapchain,
                                                VK::Format &ChosenFormat, uint32_t &OutImageCount, uvec2 &OutExtent,
                                                std::vector<VK::SwapchainImage> &OutImages)
{
    const auto SurfaceCapabilities =
        GraphicsEngine::Get().GetMainGPU()->VK()->LogicalDevice()->GetSurfaceCapabilities(surface);
    OutImageCount = std::min(SurfaceCapabilities.minImageCount + 1,
                             SurfaceCapabilities.maxImageCount != 0 ? SurfaceCapabilities.maxImageCount : UINT32_MAX);

    OutExtent = VkToGlm(SurfaceCapabilities.currentExtent);
    uvec2 MaxExtent = VkToGlm(SurfaceCapabilities.maxImageExtent),
          MinExtent = VkToGlm(SurfaceCapabilities.minImageExtent);
    OutExtent = clamp(OutExtent, MinExtent, MaxExtent);
    const auto formats =
        GraphicsEngine::Get().GetMainGPU()->VK()->LogicalDevice()->GetVkPhysicalDeviceHandle().getSurfaceFormatsKHR(
            surface);
    vk::SwapchainCreateInfoKHR CreateInfo;
    CreateInfo.clipped = true;
    CreateInfo.imageArrayLayers = 1;
    VK::Format format = VK::Format::eBGRA8_UNorm;
    CreateInfo.imageFormat = (vk::Format)format;
    CreateInfo.imageColorSpace = vk::ColorSpaceKHR::eSrgbNonlinear;
    CreateInfo.presentMode = vk::PresentModeKHR::eImmediate;
    CreateInfo.imageExtent = GlmToVkExtent(OutExtent);
    CreateInfo.minImageCount = OutImageCount;
    CreateInfo.imageSharingMode = vk::SharingMode::eExclusive;
    CreateInfo.preTransform = vk::SurfaceTransformFlagBitsKHR::eIdentity;
    CreateInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
    CreateInfo.surface = surface;
    CreateInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransferSrc |
                            vk::ImageUsageFlagBits::eTransferDst;
    CreateInfo.oldSwapchain = OldSwapchain.value_or(VK_NULL_HANDLE);
    const auto CreateResult =
        GraphicsEngine::Get().GetMainGPU()->VK()->LogicalDevice()->GetHandle().createSwapchainKHR(CreateInfo);
    LASSERT(CreateResult.result == vk::Result::eSuccess, "SHIT");

    vk::SwapchainKHR Handle = CreateResult.value;
    const auto &Retrievedimages =
        GraphicsEngine::Get().GetMainGPU()->VK()->LogicalDevice()->GetHandle().getSwapchainImagesKHR(Handle).value;
    OutImages.clear();
    OutImages.reserve(Retrievedimages.size());
    for (const auto &vkimage : Retrievedimages)
    {
        OutImages.emplace_back();
        SwapchainImage &Image = OutImages.back();
        Image.imageRaw = vkimage;
        Image.imageView.MakeRef(vkimage,format, ImageViewProperties{.AspectMask = VK::ImageAspect::eColor});
    }
    return Handle;
}

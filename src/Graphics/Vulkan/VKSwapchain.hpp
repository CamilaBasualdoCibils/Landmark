#pragma once
#include "Graphics/Synchronization.hpp"
#include "Graphics/Vulkan/Images/VKImageView.hpp"
#include "Graphics/Vulkan/Images/VKSampler.hpp"
#include "Graphics/Vulkan/VKQueue.hpp"
#include <pch.h>
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
    VK::Format ImageFormat;
    uvec2 CurrentExtent;
  public:
    Swapchain(vk::SurfaceKHR surface);
    ~Swapchain();
    vk::SwapchainKHR GetHandle() const
    {
        return vkSwapchainHandle;
    }
    operator vk::SwapchainKHR() const
    {
        return vkSwapchainHandle;
    }
    void AcquireNextSwapchainImage(GPURef<Graphics::Semaphore> onCompleteSemaphore);
    [[nodiscard]] std::pair<GPURef<Graphics::Semaphore>,SwapchainImage> AcquireNextSwapchainImage();
    void Present(GPURef<VK::Queue> queue, GPURef<Graphics::Semaphore> WaitSemaphore)
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
        LASSERT(PresentResultOverall == vk::Result::eSuccess && PresentResult == vk::Result::eSuccess, "oh man");
    }
    const auto& GetCurrentImage() const {return GetImage(CurrentImageIndex);}
    const SwapchainImage& GetImage(uint32_t index) const {return Images[index];}
    [[nodiscard]] VK::Format GetImageFormat() const {return ImageFormat;}
    [[nodiscard]] uvec2 GetExtent() const {return CurrentExtent;}

};
} // namespace VK
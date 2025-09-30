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
    GPURef<VK::ImageView> imageView;
    GPURef<VK::Sampler> sampler;
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
    bool OutOfDate = false;
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
    bool IsOutOfDate() const {return OutOfDate;}
    void AcquireNextSwapchainImage(GPURef<Graphics::Semaphore> onCompleteSemaphore);
    [[nodiscard]] std::pair<GPURef<Graphics::Semaphore>,SwapchainImage> AcquireNextSwapchainImage();
    void Present(GPURef<VK::Queue> queue, GPURef<Graphics::Semaphore> WaitSemaphore);
    const auto& GetCurrentImage() const {return GetImage(CurrentImageIndex);}
    const SwapchainImage& GetImage(uint32_t index) const {return Images[index];}
    [[nodiscard]] VK::Format GetImageFormat() const {return ImageFormat;}
    [[nodiscard]] uvec2 GetExtent() const {return CurrentExtent;}
    void Recreate(vk::SurfaceKHR surface);
private:
    [[nodiscard]] static vk::SwapchainKHR CreateSwapchain(vk::SurfaceKHR surface, std::optional<vk::SwapchainKHR> OldSwapchain,VK::Format& ChosenFormat,uint32_t& OutImageCount,uvec2& OutExtent,std::vector<VK::SwapchainImage>& OutImages);

};
} // namespace VK
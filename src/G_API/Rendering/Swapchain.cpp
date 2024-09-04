#include "Swapchain.h"
#include <G_API/Devices/LogicalDevice.h>

Swapchain::Swapchain(const SwapchainProperties &p, const RenderPass &rp, vk::SurfaceKHR surf) : properties(p)
{
    Create(rp, surf);
}

void Swapchain::Destroy()
{
    for (auto &f : framebuffers)
        f.Destroy();
    framebuffers.clear();
    GetDevice()->destroySwapchainKHR(swapchain);
}

uint32_t Swapchain::AcquireNextImage(std::optional<Semaphore> triggerSemaphore, std::optional<Fence> triggerFence, uint64_t TimeOutWait)
{
   
	vk::Semaphore semaphore = nullptr;
	if (triggerSemaphore.has_value())
		semaphore = triggerSemaphore->GetVkSemaphore();
	vk::Fence fence = nullptr;
	if (triggerFence.has_value())
		fence = triggerFence->GetVkFence();
	auto result = GetDevice()->acquireNextImageKHR(swapchain, TimeOutWait, semaphore, fence);

	if (result.result ==vk::Result::eErrorOutOfDateKHR)
		OutOfDate = true;
		else
		OutOfDate = false;

	return result.value;
}

void Swapchain::Create(const RenderPass &rp, vk::SurfaceKHR surf, std::optional<vk::SwapchainKHR> existing)
{
    //************** SWAPCHAIN **************
    vk::SwapchainCreateInfoKHR swapchain_create_info;
    swapchain_create_info.minImageCount = properties.imageCount.value_or(DeduceImageCount(surf));
    uvec2 extent = properties.extent.value_or(DeduceExtent(surf));
    swapchain_create_info.imageExtent = vk::Extent2D{extent.x, extent.y};
    swapchain_create_info.imageFormat = EnumCast(properties.colorFormat);
    swapchain_create_info.imageColorSpace = EnumCast(properties.colorSpace);
    swapchain_create_info.imageArrayLayers = 1;
    swapchain_create_info.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;

    std::vector<uint32_t> queueFamilyIndicies;
    if (properties.sharingQueues.has_value())
    {
        for (const auto &q : properties.sharingQueues.value())
            queueFamilyIndicies.push_back(q.GetFamilyID());

        swapchain_create_info.imageSharingMode = EnumCast(SharingMode::CONCURRENT);
        swapchain_create_info.queueFamilyIndexCount = queueFamilyIndicies.size();
        swapchain_create_info.pQueueFamilyIndices = queueFamilyIndicies.data();
    }
    else
        swapchain_create_info.imageSharingMode = EnumCast(SharingMode::EXCLUSIVE);

    swapchain_create_info.preTransform = EnumCast(properties.transform);
    swapchain_create_info.compositeAlpha = EnumCast(properties.alphaMode);
    swapchain_create_info.presentMode = EnumCast(properties.presentMode);
    swapchain_create_info.clipped = properties.clipped;
    swapchain_create_info.surface = surf;
    
    if (existing.has_value())
        swapchain_create_info.setOldSwapchain(existing.value());
    //swapchain_create_info.oldSwapchain = existing.value_or(nullptr);
        
    swapchain = GetDevice()->createSwapchainKHR(swapchain_create_info).value;
    if (existing.has_value())
        GetDevice()->destroySwapchainKHR(existing.value());

    /**************** fRAMEBUFFERS************ */
    if (existing.has_value())
    {
        for (auto &fbo : framebuffers)
            fbo.Destroy();
        framebuffers.clear();
    }
    const auto &images = GetDevice()->getSwapchainImagesKHR(swapchain).value;
    framebuffers.reserve(images.size());

    Image::ImageProperties imageProperties;
    imageProperties.format = properties.colorFormat;
    imageProperties.dimensions = uvec3(extent, 1);
    imageProperties.layers = 1;
    imageProperties.Samples = 1;
    imageProperties.mipLevels = 1;

    ImageView::ImageViewProperties ivP;
    Sampler::SamplerProperties sP;

    for (const auto &i : images)
    {
        Image image(i, imageProperties);
        CombinedImageSampler CImage(image, ivP, sP);
        Framebuffer framebuffer(std::vector<CombinedImageSampler>{CImage}, rp);
        framebuffers.push_back(framebuffer);
    }
}

uint32_t Swapchain::DeduceImageCount(vk::SurfaceKHR surf)
{
    const auto capabilities = GetDevice().GetSurfaceCapabilities(surf);
    uint32_t imageCount = capabilities.minImageCount + 1;
    if (capabilities.maxImageCount > 0)
        imageCount = std::min(imageCount, capabilities.maxImageCount);
    return imageCount;
}

uvec2 Swapchain::DeduceExtent(vk::SurfaceKHR surf)
{
    const auto capabilities = GetDevice().GetSurfaceCapabilities(surf);
    uvec2 extent =
        {std::clamp(capabilities.currentExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width),
         std::clamp(capabilities.currentExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height)};
    return extent;
}

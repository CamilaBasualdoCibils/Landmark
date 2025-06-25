#include "Swapchain.h"
#include <VK/Devices/LogicalDevice.h>

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

    OutOfDate = result.result != vk::Result::eSuccess;

    return result.value;
}

void Swapchain::Recreate(const RenderPass &rp, vk::SurfaceKHR surf, const SwapchainProperties &_new_prop)
{
    properties = _new_prop;
    Create(rp, surf, swapchain);
}

void Swapchain::Create(const RenderPass &rp, vk::SurfaceKHR surf, std::optional<vk::SwapchainKHR> existing)
{
    //************** SWAPCHAIN **************
    vk::SwapchainCreateInfoKHR swapchain_create_info;
    if (!properties.imageCount.has_value())
        properties.imageCount = DeduceImageCount(surf);
    swapchain_create_info.minImageCount = *properties.imageCount;
    
    uvec2 extent = properties.extent.value_or(DeduceExtent(surf));
    properties.extent = extent;
    swapchain_create_info.imageExtent = vk::Extent2D{extent.x, extent.y};
    swapchain_create_info.imageFormat = enum_cast(properties.colorFormat);
    swapchain_create_info.imageColorSpace = enum_cast(properties.colorSpace);
    swapchain_create_info.imageArrayLayers = 1;
    swapchain_create_info.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;

    std::vector<uint32_t> queueFamilyIndicies;
    if (properties.sharingQueues.has_value())
    {
        for (const auto &q : properties.sharingQueues.value())
            queueFamilyIndicies.push_back(q.GetFamilyID());

        swapchain_create_info.imageSharingMode = enum_cast(SharingMode::CONCURRENT);
        swapchain_create_info.queueFamilyIndexCount = queueFamilyIndicies.size();
        swapchain_create_info.pQueueFamilyIndices = queueFamilyIndicies.data();
    }
    else
        swapchain_create_info.imageSharingMode = enum_cast(SharingMode::EXCLUSIVE);

    swapchain_create_info.preTransform = enum_cast(properties.transform);
    swapchain_create_info.compositeAlpha = enum_cast(properties.alphaMode);
    swapchain_create_info.presentMode = enum_cast(properties.presentMode);
    swapchain_create_info.clipped = properties.clipped;
    swapchain_create_info.surface = surf;

    if (existing.has_value())
        swapchain_create_info.setOldSwapchain(existing.value());
    // swapchain_create_info.oldSwapchain = existing.value_or(nullptr);

    swapchain = GetDevice()->createSwapchainKHR(swapchain_create_info).value;

    /**************** fRAMEBUFFERS************ */
    if (existing.has_value())
    {
        GetDevice()->destroySwapchainKHR(existing.value());
        for (auto &fbo : framebuffers)
            fbo.DestroyFBOOnly();
        logger.Debug("ImageViews and Samplers leaked");
        framebuffers.clear();
        existing.reset();
    }
    const auto &images = GetDevice()->getSwapchainImagesKHR(swapchain).value;
    LASSERT(!images.empty(), "Images Empty??");
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
    OutOfDate = false;
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

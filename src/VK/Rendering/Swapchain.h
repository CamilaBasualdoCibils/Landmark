#pragma once
#include <pch.h>
#include <VK/Devices/DeviceResource.h>
#include "Framebuffer.h"
#include <VK/Operations/Queue.h>
#include <VK/Synchronization/Semaphore.h>
#include <VK/Synchronization/Fence.h>
class Swapchain : public DeviceResource
{

public:
    struct SwapchainProperties
    {
        ColorSpace colorSpace = ColorSpace::SRGB_NON_LINEAR;
        ColorFormat colorFormat = ColorFormat::RGBA8SRGB;
        PresentModes presentMode;

        // leave empty for swapchain to autodeduce imageCount
        std::optional<uint32_t> imageCount;

        // leave empty for swapchain to autodeduce extent
        std::optional<uvec2> extent;

        SurfaceTransformFlags transform = SurfaceTransformFlags::INHERIT;
        CompositeAlphaModes alphaMode = CompositeAlphaModes::OPAQUE;
        bool clipped = true;

        // if left empty then sharing mode will be exclusive
        std::optional<std::vector<Queue>> sharingQueues;
    };

private:
    vk::SwapchainKHR swapchain;
    std::vector<Framebuffer> framebuffers;
    SwapchainProperties properties;
    bool OutOfDate = false;

public:
    Swapchain(const SwapchainProperties &p, const RenderPass &rp, vk::SurfaceKHR surf);
    void Destroy() override;
    bool GetIsOutOfDate() const {return OutOfDate;}
    vk::SwapchainKHR GetVkSwapchain() const {return swapchain;}
    uint32_t AcquireNextImage(std::optional<Semaphore> triggerSemaphore = {},std::optional<Fence> triggerFence = {}, uint64_t TimeOutWait = UINT64_MAX);
    Framebuffer& GetFramebuffer(uint32_t index) {return framebuffers[index];}
    uvec2 GetExtent() const {return properties.extent.value();}
    const SwapchainProperties& GetProperties() const {return properties;}
    void Recreate(const SwapchainProperties&) {LASSERT(false,"NOT IMPLEMENTED");}
private:
    void Create(const RenderPass &rp, vk::SurfaceKHR surf, std::optional<vk::SwapchainKHR> = {});
    uint32_t DeduceImageCount(vk::SurfaceKHR surf);
    uvec2 DeduceExtent(vk::SurfaceKHR surf);
};
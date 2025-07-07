#include "RenderTarget.hpp"
#include <vulkan/vulkan_structs.hpp>

VK::RenderTarget::RenderTarget()
{
    vk::RenderingInfoKHR renderingInfo{};
renderingInfo.renderArea;
renderingInfo.layerCount = 1;
renderingInfo.colorAttachmentCount = 1;
renderingInfo.pColorAttachments;
}
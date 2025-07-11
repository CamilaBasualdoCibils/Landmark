#include "RenderTarget.hpp"
#include <misc/Conversions.hpp>
VK::RenderTarget::RenderTarget()
{
}
std::shared_ptr<std::pair<vk::RenderingInfo, std::vector<vk::RenderingAttachmentInfo>>> VK::RenderTarget::
    AsRenderingInfo() const
{
    auto ptr = std::make_shared<std::pair<vk::RenderingInfo, std::vector<vk::RenderingAttachmentInfo>>>();
    std::vector<vk::RenderingAttachmentInfo> &Attachments = ptr->second;
    vk::RenderingInfoKHR &renderingInfo = ptr->first;
    uint32_t ColorAttachmentCount = 0;
    for (const auto &ColorAttach : ColorAttachments)
    {
        if (!ColorAttach)
            break;
        Attachments.emplace_back();
        auto &Attachment = Attachments.back();
        Attachment.imageLayout = vk::ImageLayout::eGeneral;
        Attachment.imageView = *ColorAttach->GetImageView();
        Attachment.loadOp = vk::AttachmentLoadOp::eLoad;
        Attachment.storeOp = vk::AttachmentStoreOp::eStore;
        ColorAttachmentCount++;
    }
    const uint32_t DepthAttachmentPos = Attachments.size();
    if (DepthAttachment)
    {
        Attachments.emplace_back();

        auto &Attachment = Attachments.back();
         Attachment.imageLayout = vk::ImageLayout::eGeneral;
        Attachment.imageView = *DepthAttachment->GetImageView();
        Attachment.loadOp = vk::AttachmentLoadOp::eLoad;
        Attachment.storeOp = vk::AttachmentStoreOp::eStore;
    }

    renderingInfo.renderArea = GlmToVkRect(ViewportOffset, ViewportSize);
    renderingInfo.layerCount = 1;
    renderingInfo.colorAttachmentCount = ColorAttachmentCount;
    renderingInfo.pColorAttachments = Attachments.data();
    if (DepthAttachment)
    {
        renderingInfo.pDepthAttachment = Attachments.data()+DepthAttachmentPos;
    }

    return ptr;
}

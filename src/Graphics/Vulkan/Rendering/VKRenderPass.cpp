#include "VKRenderPass.hpp"
#include "Graphics/GraphicsEngine.hpp"
#include "pch.h"

VK::RenderPass::RenderPass(const VK::RenderPassProperties &Properties):Properties(Properties)
{
    std::vector<vk::AttachmentDescription> AttachmentDescriptions;
    for (int i = 0; i < Properties.Attachments.size(); i++)
    {
        const auto &Attachment = Properties.Attachments[i];
        vk::AttachmentDescription vkAttachmentDesc;
        vkAttachmentDesc.loadOp = (vk::AttachmentLoadOp)Attachment.LoadOp;
        vkAttachmentDesc.storeOp = (vk::AttachmentStoreOp)Attachment.StoreOp;
        vkAttachmentDesc.samples = vk::SampleCountFlagBits::e1;
        vkAttachmentDesc.stencilStoreOp = (vk::AttachmentStoreOp)Attachment.StencilStoreOp;
        vkAttachmentDesc.stencilLoadOp = (vk::AttachmentLoadOp)Attachment.StencilLoadOp;
        vkAttachmentDesc.format = (vk::Format)Attachment.format;
        vkAttachmentDesc.initialLayout = (vk::ImageLayout)Attachment.InitialLayout;
        vkAttachmentDesc.finalLayout = (vk::ImageLayout)Attachment.FinalLayout;
        AttachmentDescriptions.push_back(vkAttachmentDesc);
    }
    std::vector<vk::SubpassDescription> SubpassDescriptions;
    std::vector<std::vector<vk::AttachmentReference>> SubpassAttachments;
    for (int i = 0; i < Properties.Subpasses.size(); i++)
    {
        const auto &Subpass = Properties.Subpasses[i];
        SubpassAttachments.emplace_back();
        vk::SubpassDescription SubpassDescrition;
        SubpassDescrition.pipelineBindPoint = (vk::PipelineBindPoint)Subpass.BindPoint;
        for (int j = 0; j < Subpass.SubpassAttachments.size(); j++)
        {
            const auto &SubpassAttachment = Properties.Subpasses[i].SubpassAttachments[j];
            vk::AttachmentReference AttachmentReference;
            AttachmentReference.attachment = SubpassAttachment.AttachmentID;
            AttachmentReference.layout = (vk::ImageLayout)SubpassAttachment.IdealLayout;
            SubpassAttachments[i].push_back(AttachmentReference);
        }
        SubpassDescrition.setColorAttachments(SubpassAttachments[i]);
        SubpassDescriptions.push_back(SubpassDescrition);
    }

    vk::RenderPassCreateInfo CreateInfo;
    CreateInfo.setAttachments(AttachmentDescriptions);
    CreateInfo.setSubpasses(SubpassDescriptions);

    const auto CreateResult =
        GraphicsEngine::Get().GetMainGPU()->VK()->LogicalDevice()->GetHandle().createRenderPass(CreateInfo);
    LASSERT(CreateResult.result == vk::Result::eSuccess, "my bad");

    Handle = CreateResult.value;
}
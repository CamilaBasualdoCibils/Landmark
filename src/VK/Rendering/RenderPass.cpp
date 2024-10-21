#include "pch.h"
#include "RenderPass.h"
#include <VK/Devices/LogicalDevice.h>
/*
RenderPass::RenderPass(const std::vector< AttachmentDescription>& attachments,
	const std::vector<SubPassDescription>& subpasses)
{
	std::vector<vk::AttachmentDescription> attachment_descriptions(attachments.size());

	for (int i = 0; i < attachments.size(); i++)
	{
		const auto& a = attachments[i];
		auto& aDst = attachment_descriptions[i];

		aDst.format = static_cast<vk::Format>(a.format);
		aDst.samples = static_cast<vk::SampleCountFlagBits>(a.sampleCountFlag);

		aDst.loadOp = static_cast<vk::AttachmentLoadOp>(a.loadOp);
		aDst.storeOp = static_cast<vk::AttachmentStoreOp>(a.storeOp);

		aDst.stencilLoadOp = static_cast<vk::AttachmentLoadOp>(a.stencilLoadOp);
		aDst.stencilStoreOp = static_cast<vk::AttachmentStoreOp>(a.stencilStoreOp);

		aDst.finalLayout = static_cast<vk::ImageLayout>(a.finalLayout);
		aDst.initialLayout = static_cast<vk::ImageLayout>(a.initialLayout);

	}

	std::vector<vk::SubpassDescription> subpass_descriptions(subpasses.size());
	std::vector<std::vector<vk::AttachmentReference>> attachment_referenceses(subpasses.size());
	for (int i = 0; i < subpasses.size(); i++)
	{
		auto& subpass = subpass_descriptions[i];
		auto& refStorage = attachment_referenceses[i];


		for (int i = 0; i < attachment_descriptions.size();i++)
		{
			refStorage.emplace_back();
			auto& ref = refStorage.back();
			ref.attachment = i;
			ref.layout = static_cast<vk::ImageLayout>(attachments[i].intendedUse);
		}

		subpass.colorAttachmentCount = attachment_referenceses.
	}


}
*/
RenderPass::RenderPass(ColorFormat swapchainFormat)
{
	vk::AttachmentDescription colorAttachment{};
	colorAttachment.format = static_cast<vk::Format>( swapchainFormat);
	colorAttachment.samples = vk::SampleCountFlagBits::e1;

	colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
	colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;

	colorAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
	colorAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;

	colorAttachment.initialLayout = vk::ImageLayout::eUndefined;
	colorAttachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;

	vk::AttachmentReference colorAttachmentRef{};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = vk::ImageLayout::eColorAttachmentOptimal;

	vk::SubpassDescription subpass{};
	subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;

	vk::RenderPassCreateInfo renderPassInfo{};
	renderPassInfo.attachmentCount = 1;
	renderPassInfo.pAttachments = &colorAttachment;
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;

	renderPass = GetDevice()->createRenderPass(renderPassInfo).value;
	logger.Debug( "Renderpass Created. TODO: Make this more descriptive");
}

void RenderPass::Destroy()
{
	GetDevice()->destroyRenderPass(renderPass);
}

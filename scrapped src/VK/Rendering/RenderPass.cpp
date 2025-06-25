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
RenderPass::RenderPass(const Renderpass_Properties &_p) : properties(_p)
{
	std::vector<vk::AttachmentDescription> color_attachments;
	for (int i = 0; i < properties.color_attachments.size(); i++)
	{
		const attachment &att = properties.color_attachments[i];
		vk::AttachmentDescription vk_at;
		vk_at.format = enum_cast(att.format);
		vk_at.samples = enum_cast(att.sampleCountFlag);
		vk_at.loadOp = enum_cast(att.loadOp);
		vk_at.storeOp = enum_cast(att.storeOp);
		vk_at.stencilLoadOp = enum_cast(att.stencilLoadOp);
		vk_at.stencilStoreOp = enum_cast(att.stencilStoreOp);
		vk_at.initialLayout = enum_cast(att.initialLayout);
		vk_at.finalLayout = enum_cast(att.finalLayout);
		color_attachments.push_back(vk_at);
	}
	std::vector<vk::SubpassDescription> subpasses;
	using att_ref_list = std::vector<vk::AttachmentReference>;
	std::map<uint32_t,att_ref_list> subpass_references;
	for (uint32_t i = 0; i < properties.subpasses.size(); i++) {

		const subpass& subp = properties.subpasses[i];
		subpass_references.insert(std::make_pair(i,att_ref_list()));
		att_ref_list& att_list = subpass_references[i];
		vk::SubpassDescription vk_subp;
		vk_subp.pipelineBindPoint = enum_cast(subp.bind_point);

		for (uint32_t j = 0; j < subp.color_attachments.size();j++) {
			const subpass_attachment& subpass_att = subp.color_attachments[j];
			vk::AttachmentReference vk_subatt;
			vk_subatt.attachment = subpass_att.attachment;
			vk_subatt.layout = enum_cast(subpass_att.ideal_layout);
			att_list.push_back(vk_subatt);
		}
		vk_subp.colorAttachmentCount = att_list.size();
		vk_subp.pColorAttachments = att_list.data();
		subpasses.push_back(vk_subp);

	}
	
	vk::RenderPassCreateInfo render_pass_info;
	render_pass_info.setAttachments(color_attachments);
	render_pass_info.setSubpasses(subpasses);
	const auto result = GetDevice()->createRenderPass(render_pass_info);
	renderPass = result.value;
}
/*
RenderPass::RenderPass(ColorFormat swapchainFormat)
{
	vk::AttachmentDescription colorAttachment{};
	colorAttachment.format = static_cast<vk::Format>(swapchainFormat);
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
	logger.Debug("Renderpass Created. TODO: Make this more descriptive");
}
*/
void RenderPass::Destroy()
{
	GetDevice()->destroyRenderPass(renderPass);
}

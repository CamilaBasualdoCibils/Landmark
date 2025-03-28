#pragma once

#include "GenericEnums.h"
#include "VK/Devices/DeviceResource.h"
//#include "VK/Operations/CommandBuffer.h"

class RenderPass : public DeviceResource
{
public:
	using attachmentID = uint32_t;
	struct attachment
	{
		Format format = Format::BGRA_8_UNORM;
		SampleCountFlags sampleCountFlag = SampleCountFlags::e1;
		// operations
		AttachmentLoadOp loadOp = AttachmentLoadOp::CLEAR;
		AttachmentStoreOp storeOp = AttachmentStoreOp::STORE;

		// stencil operations
		AttachmentLoadOp stencilLoadOp = AttachmentLoadOp::DONT_CARE;
		AttachmentStoreOp stencilStoreOp = AttachmentStoreOp::DONT_CARE;

		ImageLayouts initialLayout = ImageLayouts::UNDEFINED;
		ImageLayouts finalLayout = ImageLayouts::PRESENT_SRC;

	};
	struct subpass_attachment {
		attachmentID attachment;
		ImageLayouts ideal_layout;
	};
	struct subpass
	{
		PipelineBindPoint bind_point = PipelineBindPoint::GRAPHICS;
		std::vector<subpass_attachment> color_attachments;
	};
	struct Renderpass_Properties
	{
		std::vector<attachment> color_attachments;
		std::vector<subpass> subpasses;
	};

private:
	vk::RenderPass renderPass;
	Renderpass_Properties properties;
	public:
	RenderPass(const Renderpass_Properties &_p);
	//RenderPass(ColorFormat swapchainFormat);
	vk::RenderPass GetVkRenderPass() { return renderPass; }
	vk::RenderPass GetVkRenderPass() const { return renderPass; }
	const Renderpass_Properties& getProperties()const {return properties;}
	operator vk::RenderPass()
	{
		return renderPass;
	}
	void Destroy() override;
};

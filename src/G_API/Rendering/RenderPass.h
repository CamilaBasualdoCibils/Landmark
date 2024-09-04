#pragma once
#ifndef _RENDERPASS_H_
#define _RENDERPASS_H_

#include "G_API/GenericEnums.h"
#include "G_API/Devices/DeviceResource.h"
#include "G_API/Operations/CommandBuffer.h"


	class RenderPass final : public DeviceResource
	{
	public:
		vk::RenderPass renderPass;
		/*
		struct AttachmentDescription
		{
			ColorFormat format;
			SampleCountFlags sampleCountFlag = SampleCountFlags::e1;

			//operations
			AttachmentLoadOp loadOp = AttachmentLoadOp::CLEAR;
			AttachmentStoreOp storeOp = AttachmentStoreOp::STORE;

			//stencil operations
			AttachmentLoadOp stencilLoadOp = AttachmentLoadOp::DONT_CARE;
			AttachmentStoreOp stencilStoreOp = AttachmentStoreOp::DONT_CARE;

			ImageLayout initialLayout = ImageLayout::UNDEFINED;
			ImageLayout finalLayout = ImageLayout::PRESENT_SRC;

			//TODO: rework this, subpasses might use an attachment for different purposes
			ImageLayout intendedUse = ImageLayout::COLOR_ATTACHMENT;
		};
		struct SubPassDescription
		{
			
		}; 
		RenderPass(const std::vector< AttachmentDescription>& attachments,const std::vector<SubPassDescription>& subpasses);
		*/

		RenderPass(ColorFormat swapchainFormat);
		vk::RenderPass GetVkRenderPass() { return renderPass; }
		vk::RenderPass GetVkRenderPass() const { return renderPass; }

		void Destroy() override;
		
	};

#endif
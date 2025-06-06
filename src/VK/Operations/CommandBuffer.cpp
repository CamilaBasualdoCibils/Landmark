﻿#include "pch.h"
#include "CommandBuffer.h"

#include "Queue.h"
#include <VK/Devices/LogicalDevice.h>

#include "VK/Rendering/RenderPass.h"
#include <VK/Rendering/Framebuffer.h>
#include <VK/Rendering/Pipeline.h>
#include <VK/Descriptors/DescriptorSet.h>
CommandBuffer::SubmitInfo CommandBuffer::prepareSubmitInfo(const std::vector<Semaphore> &waitSemaphores,
														   const std::vector<Semaphore> &signalSemaphores)
{
	SubmitInfo finalsubmit;

	finalsubmit.submit_info.commandBufferCount = 1;
	finalsubmit.submit_info.pCommandBuffers = &cmdbuffer;

	{
		for (auto &ss : signalSemaphores)
		{
			finalsubmit.signalSemaphores.push_back(ss);
		}
	}

	{
		for (auto &ss : waitSemaphores)
		{
			finalsubmit.waitSemaphores.push_back(ss);
		}
	}
	finalsubmit.submit_info.signalSemaphoreCount = finalsubmit.signalSemaphores.size();
	finalsubmit.submit_info.pSignalSemaphores = finalsubmit.signalSemaphores.data();
	finalsubmit.submit_info.waitSemaphoreCount = finalsubmit.waitSemaphores.size();
	const static vk::PipelineStageFlags waitStage = vk::PipelineStageFlagBits::eColorAttachmentOutput;
	finalsubmit.submit_info.pWaitDstStageMask = &waitStage;
	finalsubmit.submit_info.pWaitSemaphores = finalsubmit.waitSemaphores.data();

	return finalsubmit;
}

CommandBuffer::CommandBuffer(CommandPool &cp) : pool(cp)
{

	vk::CommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = vk::StructureType::eCommandBufferAllocateInfo;
	allocInfo.level = vk::CommandBufferLevel::ePrimary;
	allocInfo.commandPool = pool.GetVkCommandPool();
	allocInfo.commandBufferCount = 1;
	cmdbuffer = GetvkDevice().allocateCommandBuffers(allocInfo).value.front();
}
/*
CommandBuffer::~CommandBuffer()
{

	static auto& GraphicsPool = RenderAPI::GetInstance()->GetGraphicsCmdPool();
	//vkQueueWaitIdle(queue);
	GetvkDevice().freeCommandBuffers(GraphicsPool, { cmdbuffer });
	//GraphicsQueue.freeCommandBuffers(GraphicsPool, { cmd });
	//vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
}
*/
void CommandBuffer::Begin(Flags<CommandBufferUsageFlags> flag)
{
	vk::CommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = vk::StructureType::eCommandBufferBeginInfo;
	beginInfo.flags = flag;
	auto result = cmdbuffer.begin(beginInfo);
	has_begun = true;
}

void CommandBuffer::End()
{
	auto result = cmdbuffer.end();
	has_begun = false;
}

void CommandBuffer::BeginRenderPass(RenderPass &rp, Framebuffer &fb, vk::Rect2D renderArea,
									bool ClearFramebuffer, vk::ClearValue clearValues)
{
	vk::RenderPassBeginInfo render_pass_begin;
	render_pass_begin.framebuffer = fb.GetVkFramebuffer();
	render_pass_begin.renderPass = rp.GetVkRenderPass();
	render_pass_begin.clearValueCount = ClearFramebuffer + 1;
	render_pass_begin.pClearValues = &clearValues;
	render_pass_begin.renderArea = renderArea;

	(*this)->beginRenderPass(render_pass_begin, vk::SubpassContents::eInline);
}

void CommandBuffer::EndRenderPass()
{
	(*this)->endRenderPass();
}

void CommandBuffer::Reset()
{
	cmdbuffer.reset();
	has_begun = false;
}

void CommandBuffer::Submit(const std::vector<Semaphore> &waitSemaphores,
						   const std::vector<Semaphore> &signalSemaphores, const std::optional<Fence> &CompletionFence)
{
	auto submit = prepareSubmitInfo(waitSemaphores, signalSemaphores);

	vk::Fence f = {};
	if (CompletionFence.has_value())
		f = CompletionFence.value();

	auto result = pool.GetBelongingQueue()->submit(submit.submit_info, f);
}

void CommandBuffer::SubmitAndWait(const std::vector<Semaphore> &waitSemaphores,
								  const std::vector<Semaphore> &signalSemaphores)
{
	auto submit = prepareSubmitInfo(waitSemaphores, signalSemaphores);

	auto result = pool.GetBelongingQueue()->submit(submit.submit_info, fence);
	fence.WaitAndReset();
}

void CommandBuffer::CopyBuffer(Buffer &srcBuffer, Buffer &dstBuffer, uint64_t size, uint64_t srcOffset, uint64_t dstOffset)
{
	vk::BufferCopy copy_region(srcOffset, dstOffset, size);
	cmdbuffer.copyBuffer(srcBuffer, dstBuffer, 1, &copy_region);
}

void CommandBuffer::BindPipeline(const Pipeline &p, PipelineBindPoint bp)
{
	cmdbuffer.bindPipeline(enum_cast(bp), p);
}

void CommandBuffer::BindDescriptorSets(const PipelineLayout &layout, const std::vector<DescriptorSet> &descriptor_sets, uint32_t first_set, uint32_t binding, PipelineBindPoint bind_point)
{
	std::vector<vk::DescriptorSet> vk_ds;
	for (const auto &ds : descriptor_sets)
	{
		vk_ds.push_back(ds);
	}
	cmdbuffer.bindDescriptorSets((vk::PipelineBindPoint)bind_point,
								 (vk::PipelineLayout)layout,
								 first_set,
								 vk_ds,
								 {});
}

void CommandBuffer::Destroy()
{
	GetDevice()->freeCommandBuffers(pool.GetVkCommandPool(), {cmdbuffer});
	fence.Destroy();
}

void CommandBuffer::SetViewport(Viewport bounds)
{

	cmdbuffer.setViewport(0, {bounds});
}

void CommandBuffer::SetScissor(IRect2D bounds)
{
	vk::Rect2D rect;
	rect.offset.x = bounds.min.x;
	rect.offset.y = bounds.min.y;
	rect.extent.width = bounds.size().x;
	rect.extent.height = bounds.size().y;
	cmdbuffer.setScissor(0,{rect});
}

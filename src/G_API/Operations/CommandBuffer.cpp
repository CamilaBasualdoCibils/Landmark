#include "pch.h"
#include "CommandBuffer.h"


#include "Queue.h"
#include <G_API/Devices/LogicalDevice.h>

#include "G_API/Rendering/RenderPass.h"
#include <G_API/Rendering/Framebuffer.h>
#include <G_API/Rendering/Pipeline.h>
CommandBuffer::SubmitInfo CommandBuffer::prepareSubmitInfo(const std::vector<Semaphore>& waitSemaphores,
                                                                                                   const std::vector<Semaphore>& signalSemaphores)
{
	SubmitInfo finalsubmit;

	finalsubmit.submit_info.commandBufferCount = 1;
	finalsubmit.submit_info.pCommandBuffers = &cmdbuffer;

	{
		for (auto& ss : signalSemaphores)
		{
			finalsubmit.signalSemaphores.push_back(ss);
		}
	}

	{
		for (auto& ss : waitSemaphores)
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

CommandBuffer::CommandBuffer(CommandPool& cp):pool(cp)
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
}

void CommandBuffer::End()
{
	auto result = cmdbuffer.end();
	
}

void CommandBuffer::BeginRenderPass(RenderPass& rp, Framebuffer& fb, vk::Rect2D renderArea,
	bool ClearFramebuffer, vk::ClearValue clearValues)
{
	vk::RenderPassBeginInfo render_pass_begin;
	render_pass_begin.framebuffer = fb.GetVkFramebuffer();
	render_pass_begin.renderPass = rp.GetVkRenderPass();
	render_pass_begin.clearValueCount = ClearFramebuffer+1;
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
}

void CommandBuffer::Submit(const std::vector<Semaphore>& waitSemaphores,
                                               const std::vector<Semaphore>& signalSemaphores, const std::optional<Fence>& CompletionFence)
{
	auto submit = prepareSubmitInfo(waitSemaphores, signalSemaphores);


	vk::Fence f = {};
	if (CompletionFence.has_value()) f = CompletionFence.value();
	
	auto result = pool.GetBelongingQueue()->submit(submit.submit_info, f);
}

void CommandBuffer::SubmitAndWait(const std::vector<Semaphore>& waitSemaphores,
	const std::vector<Semaphore>& signalSemaphores)
{
	auto submit = prepareSubmitInfo(waitSemaphores, signalSemaphores);


	auto result = pool.GetBelongingQueue()->submit(submit.submit_info, fence);
	fence.WaitAndReset();
}

void CommandBuffer::BindPipeline(const Pipeline &p, PipelineBindPoint bp)
{
	cmdbuffer.bindPipeline(EnumCast(bp),p);
}

void CommandBuffer::Destroy()
{
	GetDevice()->freeCommandBuffers(pool.GetVkCommandPool(), { cmdbuffer });
}


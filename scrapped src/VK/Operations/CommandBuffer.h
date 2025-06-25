#pragma once

#include <VK/Synchronization/Fence.h>
#include <VK/Synchronization/Semaphore.h>
#include "VK/Devices/DeviceResource.h"
#include <VK/Rendering/PipelineLayout.h>
#include "CommandPool.h"
#include "GenericEnums.h"
#include "Types/Rect.hpp"
#include <Types/Viewport.h>
class Buffer;
class Framebuffer;
class Pipeline;
class RenderPass;
class DescriptorSet;
class CommandBuffer final : DeviceResource
{
	vk::CommandBuffer cmdbuffer;
	CommandPool& pool;
	Fence fence;
	bool has_begun = false;
private:

	struct SubmitInfo
	{
		vk::SubmitInfo submit_info;
		std::vector<vk::Semaphore> waitSemaphores;
		std::vector<vk::Semaphore> signalSemaphores;
	};
	SubmitInfo prepareSubmitInfo(const std::vector<Semaphore>& waitSemaphores = {}, const std::vector<Semaphore>& signalSemaphores = {});

public:
	CommandBuffer(CommandPool&);
	//~CommandBuffer();
	operator vk::CommandBuffer()
	{
		return cmdbuffer;
	}
	
	void Begin(Flags<CommandBufferUsageFlags> flag = 0);

	void End();
	constexpr bool HasBegun() const {return has_begun;}
	void BeginRenderPass(RenderPass& rp, Framebuffer& fb, vk::Rect2D renderArea, bool ClearFramebuffer = false, vk::ClearValue clearValues = {});

	void EndRenderPass();
	void Reset();
	void Submit(const std::vector<Semaphore>& waitSemaphores = {}, const std::vector<Semaphore>& signalSemaphores = {}, const std::optional<Fence>& CompletionFence = {});
	void SubmitAndWait(const std::vector<Semaphore>& waitSemaphores = {}, const std::vector<Semaphore>& signalSemaphores = {});
	void CopyBuffer(Buffer& srcBuffer, Buffer& dstBuffer,uint64_t size, uint64_t srcOffset = 0, uint64_t dstOffset = 0);
	//vk::CommandBuffer& getVkCmdBuffer() { return cmdbuffer; }
	void BindPipeline(const Pipeline& p,PipelineBindPoint bp);
	void BindDescriptorSets(const PipelineLayout& layout, const std::vector<DescriptorSet> &descriptor_sets,uint32_t first_set, uint32_t binding,PipelineBindPoint bind_point);

	void Destroy() override;
	void Draw(uint32_t vertexCount,uint32_t instanceCount = 1,uint32_t firstVertex = 0, uint32_t firstInstance = 0 ) {cmdbuffer.draw(vertexCount,instanceCount,firstVertex,firstInstance);}
	void DrawIndexed(uint32_t index_count,uint32_t instance_count = 1,uint32_t first_index = 0,int32_t vertex_offset = 0, uint32_t first_instance = 0) {
		cmdbuffer.drawIndexed(index_count,instance_count,first_index,vertex_offset,first_instance);
	}
	vk::CommandBuffer* operator->() { return &cmdbuffer; }
	void SetViewport(Viewport bounds);
	void SetScissor(IRect2D bounds);
};


#pragma once
#include <G_API/Synchronization/Fence.h>
#include <G_API/Synchronization/Semaphore.h>

#include "G_API/Devices/DeviceResource.h"
#include "CommandPool.h"
#include "G_API/GenericEnums.h"


class Framebuffer;
class Pipeline;
class RenderPass;

class CommandBuffer final : DeviceResource
{
	vk::CommandBuffer cmdbuffer;
	CommandPool& pool;
	Fence fence;
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
	operator vk::CommandBuffer() const
	{
		return cmdbuffer;
	}
	
	void Begin(Flags<CommandBufferUsageFlags> flag = 0);

	void End();
	void BeginRenderPass(RenderPass& rp, Framebuffer& fb, vk::Rect2D renderArea, bool ClearFramebuffer = false, vk::ClearValue clearValues = {});

	void EndRenderPass();
	void Reset();
	void Submit(const std::vector<Semaphore>& waitSemaphores = {}, const std::vector<Semaphore>& signalSemaphores = {}, const std::optional<Fence>& CompletionFence = {});
	void SubmitAndWait(const std::vector<Semaphore>& waitSemaphores = {}, const std::vector<Semaphore>& signalSemaphores = {});
	//vk::CommandBuffer& getVkCmdBuffer() { return cmdbuffer; }
	void BindPipeline(const Pipeline& p,PipelineBindPoint bp);
	void Destroy() override;
	void Draw(uint32_t vertexCount,uint32_t instanceCount = 1,uint32_t firstVertex = 0, uint32_t firstInstance = 0 ) {cmdbuffer.draw(vertexCount,instanceCount,firstVertex,firstInstance);}

	vk::CommandBuffer* operator->() { return &cmdbuffer; }
};


#include "pch.h"
#include "Queue.h"

#include <VK/Rendering/Swapchain.h>
Queue::Queue(LogicalDevice &dev, uint32_t _queueFamily, uint32_t _queueID) : DeviceResource(dev), QueueFamilyID(_queueFamily), QueueID(_queueID)
{
	_VkQueue = GetvkDevice().getQueue(QueueFamilyID, QueueID);
}
/*
Queue::Queue(const Queue& o) : DeviceResource(o.GetDevice()),QueueFamilyID(o.GetFamilyID()),QueueID(o.GetQueueID()),_VkQueue(o._VkQueue)
{
}

*/

bool Queue::operator<(const Queue &o) const
{
	if (GetFamilyID() != o.GetFamilyID())
		return GetFamilyID() < o.GetFamilyID();

	return GetQueueID() < o.GetQueueID();
}

void Queue::Present(uint32_t imageIndex, bool *swapchainOutOfDate, Swapchain &sc,
					std::vector<Semaphore> waitSemaphores)
{

	vk::PresentInfoKHR present_info;
	present_info.pImageIndices = &imageIndex;
	vk::SwapchainKHR swapchain = sc.GetVkSwapchain();
	present_info.pSwapchains = &swapchain;
	present_info.swapchainCount = 1;
	static std::vector<vk::Semaphore> semaphores;
	if (!waitSemaphores.empty())
	{
		semaphores.clear();
		semaphores.reserve(waitSemaphores.size());
		for (auto &semaphore : waitSemaphores)
			semaphores.push_back(semaphore.GetVkSemaphore());
	}

	present_info.pWaitSemaphores = semaphores.data();
	present_info.waitSemaphoreCount = semaphores.size();
	
	vk::Result result = (*this)->presentKHR(present_info);
	*swapchainOutOfDate = result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR;
	LASSERT(result == vk::Result::eSuccess ||
				result == vk::Result::eErrorOutOfDateKHR ||
				result == vk::Result::eSuboptimalKHR,
			"Failed to presnset");
}

void Queue::Destroy()
{
	LASSERT(false, "Queue::Destroy() does and should do nothing, improve this eventually");
}

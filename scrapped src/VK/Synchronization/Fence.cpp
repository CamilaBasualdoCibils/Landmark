#include "pch.h"
#include "Fence.h"



Fence::Fence(bool signaled)
{
	vk::FenceCreateInfo fence_create_info = {};
	fence_create_info.flags = signaled ? vk::FenceCreateFlagBits::eSignaled : vk::FenceCreateFlagBits();
	fence = GetvkDevice().createFence(fence_create_info).value;

}

void Fence::Destroy()
{
	GetvkDevice().destroyFence(fence);

}

Fence::operator vk::Fence() const
{
	return fence;
}

void Fence::Wait()
{

	auto result = GetvkDevice().waitForFences({fence},VK_TRUE,UINT64_MAX);
}

void Fence::Reset()
{
	GetvkDevice().resetFences({fence });
}

void Fence::WaitAndReset()
{
	Wait();
	Reset();
}

bool Fence::IsSignaled() const
{
	auto result = GetvkDevice().getFenceStatus(fence);

	return result == vk::Result::eSuccess;
			
}

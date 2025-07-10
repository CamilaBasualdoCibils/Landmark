#include "pch.h"
#include "CommandPool.h"
#include "Queue.h"
#include <VK/Devices/LogicalDevice.h>
#include "CommandPool.hpp"
CommandPool::CommandPool(Queue& q):queue(q)
{
	vk::CommandPoolCreateInfo create_info{};
	create_info.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
	create_info.queueFamilyIndex = queue.GetFamilyID();
	cmdPool = GetvkDevice().createCommandPool(create_info).value;


}

void CommandPool::Destroy()
{
	GetDevice()->destroyCommandPool(cmdPool);
}


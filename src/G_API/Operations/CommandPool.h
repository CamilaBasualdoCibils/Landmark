#pragma once
#include "G_API/Devices/DeviceResource.h"
class Queue;


	class CommandPool final : DeviceResource
	{
	
		vk::CommandPool cmdPool;
		Queue& queue;
	public:
		CommandPool(Queue& q);
		vk::CommandPool GetVkCommandPool() const { return cmdPool; }
		Queue& GetBelongingQueue() const { return queue; }
		void Destroy() override;
	};

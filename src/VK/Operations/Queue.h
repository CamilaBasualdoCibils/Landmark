#pragma once

#ifndef QUEUE_H_
#define QUEUE_H_
#include <pch.h>
#include "VK/Devices/DeviceResource.h"
class Swapchain;
class Semaphore;
class LogicalDevice;
class Queue final : public DeviceResource
{
	uint32_t QueueFamilyID;
	uint32_t QueueID;
	vk::Queue _VkQueue;
public:
	Queue(LogicalDevice& dev, uint32_t QueueFamily, uint32_t QueueID);
	//Queue(const Queue& o);
	//Queue& operator=(const Queue& o) = delete;

	bool operator<(const Queue& o) const;
	const vk::Queue& GetQueue() { return _VkQueue; }
	vk::Queue* operator->() { return &_VkQueue; }
	operator vk::Queue() { return _VkQueue; }

	uint32_t GetFamilyID() const { return QueueFamilyID; }
	uint32_t GetQueueID() const { return QueueID; }

	void Present(uint32_t imageIndex,bool* swapchainOutOfDate, Swapchain& sc, std::vector<Semaphore> waitSemaphores = {});

	void Destroy() override;
};

#endif
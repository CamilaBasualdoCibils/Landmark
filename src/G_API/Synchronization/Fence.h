#pragma once
#include "G_API/Devices/DeviceResource.h"

#ifndef _FENCE_H_
#define _FENCE_H_

	class Fence final : public DeviceResource
	{
		vk::Fence fence;
	public:
		Fence(bool signaled = false);
		void Destroy() override;
		operator vk::Fence() const;

		void Wait();
		void Reset();
		void WaitAndReset();

		bool IsSignaled() const;

		vk::Fence GetVkFence() const { return fence; }
	};


#endif
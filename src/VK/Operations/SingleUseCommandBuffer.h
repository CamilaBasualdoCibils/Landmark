﻿#pragma once
#ifndef _SINGLEUSECOMMANDBUFFER_H_
#define _SINGLEUSECOMMANDBUFFER_H_
#include "CommandBuffer.h"

#include "VK/Devices/DeviceResource.h"

class SingleUseCommandBuffer
{
	CommandBuffer cmdBuffer;

public:
	SingleUseCommandBuffer(CommandPool &);
	~SingleUseCommandBuffer();
	SingleUseCommandBuffer(const SingleUseCommandBuffer &o) = delete;
	SingleUseCommandBuffer &operator=(const SingleUseCommandBuffer &o) = delete;
	operator CommandBuffer&(){return cmdBuffer;}
	operator const CommandBuffer&() const { return cmdBuffer; }

	vk::CommandBuffer *operator->() { return cmdBuffer.operator->(); }
	CommandBuffer &operator*() { return cmdBuffer; }
};

#endif
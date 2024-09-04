﻿#pragma once
#include "Debug/Debug.h"
#include <algorithm>

#ifndef _DEVICEOBJECT_H_
#define _DEVICEOBJECT_H_
class LogicalDevice;

class DeviceResource
{
public:
	vk::Device GetvkDevice() const;
	LogicalDevice &GetDevice() const;

	LogicalDevice *device;

protected:
	Log logger;

	DeviceResource(LogicalDevice &_device = GetMainDevice());
	~DeviceResource();
	DeviceResource &operator=(const DeviceResource &o) = default;
	virtual void Destroy() = 0;
	static LogicalDevice &GetMainDevice();
};

#endif
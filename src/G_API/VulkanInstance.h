﻿#pragma once
#ifndef _VULKANINSTANCE_H_
#define _VULKANINSTANCE_H_

#include "Devices/MainDevice.h"

#include "Debug/Debug.h"
#include "G_API/Window.h"

class VulkanInstance
{

	static inline VulkanInstance* INSTANCE = nullptr;
	const bool ValidationEnabled;
	vk::Instance instance;
	Landmark::Window window;
	const std::vector<PhysicalDevice> physicalDevices;
	vk::DebugUtilsMessengerEXT debugMessenger;
	MainDevice mainDevice;

	

	static inline Log log = Log("Vulkan");

	
public:
	VulkanInstance(bool _ValidationEnabled);

	VulkanInstance(const VulkanInstance& o) = delete;
	VulkanInstance& operator=(const VulkanInstance&) = delete;
	static  VulkanInstance& GetInstance();
	static  Landmark::Window& GetWindow();

	MainDevice& GetMainDevice() { return mainDevice; }
	vk::Instance& GetVkInstance() { return instance; }

private:
	vk::Instance CreateInstance();
	

	std::vector<PhysicalDevice> FetchPhysicalDevices();
	const PhysicalDevice& PickMainDevice();
};
#endif
#pragma once

#ifndef _MAINDEVICE_H_
#define _MAINDEVICE_H_
#include "LogicalDevice.h"
class MainDevice : public LogicalDevice
{
	static inline Log logger = Log("Main Device");
	const static inline DeviceFeatures mainDeviceFeatures = [](){
		DeviceFeatures f;
		f.queues = {
			// Graphics Queue
		{1, {true, true, true, true, false}},
		// Compute Queue
		{1, {false, false, true, true, false}},
		// Present Queue
		{1, {false, true, false, true, false}}};
		f.wideLines = true;
		f.geometryShader = true;
		f.descriptorIndexding = true;
		f.khronosValidation = true;
		f.samplerAnistropy = true;
		f.sampleRateShading = true;
		f.swapchainSupport = true;
		f.tesselationShader = true;

		return f;
	}();
public:
	MainDevice(const PhysicalDevice &pd);
	Queue &GetGraphicsQueue()
	{
		return FetchQueue(0);
	}
	Queue &GetComputeQueue()
	{
		return FetchQueue(1);
	}
	Queue &GetPresentQueue()
	{
		return FetchQueue(2);
	}

private:
	std::vector<std::string> ChooseExtensions(const PhysicalDevice &pd) const;
	Queue &FetchQueue(uint32_t mainDeviceQueue)
	{

		return *GetQueues(mainDeviceFeatures.queues[mainDeviceQueue].capabilities.ToFlagMask()).front();
	}
};
#endif
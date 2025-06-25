#include "pch.h"
#include "MainDevice.h"

MainDevice::MainDevice(const PhysicalDevice& pd): LogicalDevice(pd, mainDeviceFeatures)
{
	//logger << "Init" << Log::end;
	logger.Debug("Init");
}



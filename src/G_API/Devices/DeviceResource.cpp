#include "pch.h"
#include "DeviceResource.h"
#include "LogicalDevice.h"
#include "G_API/VulkanInstance.h"

vk::Device DeviceResource::GetvkDevice() const
{
	LASSERT(device != nullptr, "Idiot");
	return device->GetvkDevice();
}

LogicalDevice& DeviceResource::GetDevice() const
{
	LASSERT(device != nullptr, "Idiot");
	return *device;
}



DeviceResource::DeviceResource(LogicalDevice &_device) : device(&_device), logger(device->GetName())
{


}
DeviceResource::~DeviceResource()
{
	
	

}
LogicalDevice &DeviceResource::GetMainDevice()
{
	return VulkanInstance::GetInstance().GetMainDevice();
}



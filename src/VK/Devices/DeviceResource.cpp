#include "pch.h"
#include "DeviceResource.h"
#include "LogicalDevice.h"
#include "VK/Vulkan_Instance.h"
#include "Core/App.h"
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
	MainDevice* main_device = App::GetInstance()->GetMainDevice();
	LASSERT(main_device,"Main Device doesnt exist?!");
	return *main_device;

}



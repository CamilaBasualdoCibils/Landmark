#include "pch.h"
#include "QueueFamily.h"
#include "G_API/VulkanInstance.h"
QueueFamily::QueueFamily(vk::PhysicalDevice device, const uint32_t& _familyID):
	owner(device),
familyID(_familyID),
	familyProperties(device.getQueueFamilyProperties()[familyID]),
	queueCount(familyProperties.queueCount),
	queueFlags(familyProperties.queueFlags),
	capabilities(FetchCapabilities(familyID)),
	timeStampValidBits(familyProperties.timestampValidBits),
	minImageTransferGranularity(familyProperties.minImageTransferGranularity)
{

}

QueueFamily::Capabilities QueueFamily::FetchCapabilities(const uint32_t& familyID)
{
	Capabilities capabilities;

	auto CheckFor = [&](vk::QueueFlagBits flag, bool& result)->void
	{
		auto a = (queueFlags & flag);
		if (a) result = true;
		else result = false;
	};

	CheckFor(vk::QueueFlagBits::eGraphics, capabilities.Graphics);
	CheckFor(vk::QueueFlagBits::eCompute, capabilities.Compute);
	CheckFor(vk::QueueFlagBits::eTransfer, capabilities.Transfer);
	CheckFor(vk::QueueFlagBits::eSparseBinding, capabilities.SparseBinding);
	
	capabilities.Present = owner.getSurfaceSupportKHR(familyID, VulkanInstance::GetWindow().GetSurface()).value;

	return capabilities;
}

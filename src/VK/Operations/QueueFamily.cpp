#include "pch.h"
#include "QueueFamily.h"
QueueFamily::QueueFamily(vk::PhysicalDevice device, const uint32_t& _familyID,vk::SurfaceKHR* surface):
	owner(device),
familyID(_familyID),
	familyProperties(device.getQueueFamilyProperties()[familyID]),
	queueCount(familyProperties.queueCount),
	queueFlags(familyProperties.queueFlags),
	capabilities(FetchCapabilities(familyID,surface)),
	timeStampValidBits(familyProperties.timestampValidBits),
	minImageTransferGranularity(familyProperties.minImageTransferGranularity)
{

}

QueueFamily::Capabilities QueueFamily::FetchCapabilities(const uint32_t& familyID,vk::SurfaceKHR* surface)
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
	

	if (surface)
	capabilities.Present = owner.getSurfaceSupportKHR(familyID,*surface ).value;
	else capabilities.Present = false;

	return capabilities;
}

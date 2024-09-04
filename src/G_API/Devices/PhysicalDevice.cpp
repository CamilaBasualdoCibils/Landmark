#include "pch.h"
#include "PhysicalDevice.h"

PhysicalDevice::PhysicalDevice(const vk::PhysicalDevice& pd):physicalDevice(pd),
                                                             properties(pd.getProperties()),
                                                             features(pd.getFeatures()),
                                                             memoryProperties(pd.getMemoryProperties()),
                                                             Name(properties.deviceName.data()),
                                                             UniqueID(properties.deviceID),
                                                             type(properties.deviceType),
                                                             queueFamilies(FetchQueueFamilies()),
															 AvailableExtensions(FetchExtensions())
{

}

bool PhysicalDevice::HasExtension(const std::string& name) const
{
	return AvailableExtensions.contains(name);
}

const vk::ExtensionProperties& PhysicalDevice::GetExtension(const std::string& name) const
{
	return AvailableExtensions.at(name);
}

std::vector<QueueFamily> PhysicalDevice::FetchQueueFamilies()
{
	std::vector<QueueFamily> queueFamilies;
	const auto queueFamilyCount = physicalDevice.getQueueFamilyProperties().size();
	queueFamilies.reserve(queueFamilyCount);

	for (int i = 0; i < queueFamilyCount; i++)
		queueFamilies.emplace_back(physicalDevice, i);

	return queueFamilies;
}

std::map<std::string, vk::ExtensionProperties> PhysicalDevice::FetchExtensions()
{
	auto extensions = physicalDevice.enumerateDeviceExtensionProperties().value;
	std::map < std::string, vk::ExtensionProperties > vkExtensions;
	for (auto& ext : extensions)
	{
		vkExtensions.emplace(ext.extensionName, ext);
	}
	return vkExtensions;
}
PhysicalDevice::MemoryTypeIndexInfo PhysicalDevice::GetMemoryTypeThatFits(uint32_t typefilter, Flags<MemoryProperties> bits)
{
	for (int32_t i = 0; i < memoryProperties.memoryTypeCount; i++) {
		if ((typefilter & (1 << i)) && (bits & memoryProperties.memoryTypes[i].propertyFlags) == bits) {
			return { i,memoryProperties.memoryTypes[i].propertyFlags };
		}
	}
	return { -1,vk::MemoryPropertyFlagBits(0) };
	/*
	int lastLeftOver = 999999;
	int bestfit = -1;
	for (int i = 0 ; i < memoryProperties.memoryTypeCount;i++)
	{
		const vk::MemoryType& mt = memoryProperties.memoryTypes[i];

		if ((mt.propertyFlags & bits) ==bits) //memory index fits
		{
			vk::MemoryPropertyFlags leftoverbits =( mt.propertyFlags & (~bits));



			int leftovercount = CountBits(static_cast<int>(static_cast<VkMemoryPropertyFlags>(leftoverbits)));
			if (leftovercount == 0)
			{
				MemoryTypeIndexInfo m;
				m.index = i;
				m.flags = static_cast<vk::MemoryPropertyFlags>( mt.propertyFlags);
				return m;
			}

			if (leftovercount < lastLeftOver)
			{
				lastLeftOver = leftovercount;
				bestfit = i;
			}

		}

	}

	return { bestfit,memoryProperties.memoryTypes[bestfit].propertyFlags };
	*/
}
#include "pch.h"
#include "Buffer.h"


#include "VK/Devices/LogicalDevice.h"
#include <VK/Operations/SingleUseCommandBuffer.h>
Buffer::Buffer(const BufferProperties& _prop) :DeviceResource(),properties(_prop)
{



	vk::BufferCreateInfo buffer_create_info{};
	buffer_create_info.usage = properties.usage_flags;
	buffer_create_info.size = properties.size;
	if (properties.sharing_mode.share_mode == SharingMode::CONCURRENT && properties.sharing_mode.queue_families.empty())
		logger.Error("Buffer created with sharing state Concurrent must specify Queue Families to allow access");
	buffer_create_info.sharingMode = static_cast<vk::SharingMode>(properties.sharing_mode.share_mode);
	buffer_create_info.queueFamilyIndexCount = properties.sharing_mode.queue_families.size();
	buffer_create_info.pQueueFamilyIndices = properties.sharing_mode.queue_families.data();

	buffer = GetvkDevice().createBuffer(buffer_create_info).value;

	auto memoryRequirements = GetvkDevice().getBufferMemoryRequirements(buffer);





	memory_type_index = GetDevice().GetMemoryTypeThatFits(memoryRequirements.memoryTypeBits, properties.memory_properties);

	if (!memory_type_index.isValid())
	{
		logger.Error("Buffer Memory type requirements could not be fullfilled ");
	}

	vk::MemoryAllocateInfo memory_allocate_info{};
	memory_allocate_info.allocationSize = memoryRequirements.size;
	memory_allocate_info.memoryTypeIndex = memory_type_index.index;
	bufferMemory = GetvkDevice().allocateMemory(memory_allocate_info).value;

	auto result = GetvkDevice().bindBufferMemory(buffer, bufferMemory, 0);

	std::stringstream ss;
	ss << "Buffer Allocated:\n"
		<< "	Size: " << properties.size << "\n"
		<< "	Usage: " << properties.usage_flags << "\n"
		<< "	Sharing Mode: " << vk::to_string(static_cast<vk::SharingMode>(properties.sharing_mode.share_mode)) << "\n"
		<< "	Memory Type: " << properties.memory_properties
		;
	logger.Debug(ss);

}

Buffer::~Buffer()
{

	GetvkDevice().destroyBuffer(buffer);
	GetvkDevice().freeMemory(bufferMemory);
	logger.Debug( "Buffer Unallocated");
}

void* Buffer::MapMemory()
{
	return GetvkDevice().mapMemory(bufferMemory, 0, properties.size).value;
}

void Buffer::UnmapMemory()
{
	GetvkDevice().unmapMemory(bufferMemory);
}

size_t Buffer::Size()
{
	return properties.size;
}

void Buffer::Realloc(CommandPool& cmdPool, const size_t newsize)
{
	if (!(properties.usage_flags & (BufferUsage::TRANSFER_DST | BufferUsage::TRANSFER_SRC)))
	{
		std::stringstream ss;
		ss << "Buffer Realloc is invalid. usage flags does not contain TRANSFER_DST & TRANSFER_SRC\n"
			<< "Buffer Usage: " << properties.usage_flags;
		logger.Error(ss.str());
		return;
	}
	vk::BufferCreateInfo bufferCreateInfo = {};
	//VkBufferCreateInfo bufferCreateInfo = {};
	bufferCreateInfo.sType = vk::StructureType::eBufferCreateInfo;
	bufferCreateInfo.size = newsize;  // Set the new size here
	bufferCreateInfo.usage = properties.usage_flags; // Adjust usage as needed


	//VkBuffer newBuffer;
	vk::Buffer newBuffer = GetvkDevice().createBuffer(bufferCreateInfo).value;
	//vkCreateBuffer(device, &bufferCreateInfo, nullptr, &newBuffer);


	vk::MemoryRequirements memRequirements = GetvkDevice().getBufferMemoryRequirements(newBuffer);
	//vkGetBufferMemoryRequirements(device, newBuffer, &memRequirements);

	vk::MemoryAllocateInfo allocInfo = {};
	allocInfo.sType = vk::StructureType::eMemoryAllocateInfo;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = memory_type_index.index;

	vk::DeviceMemory newBufferMemory = GetvkDevice().allocateMemory(allocInfo).value;
	//vkAllocateMemory(device, &allocInfo, nullptr, &newBufferMemory);

	auto result = GetvkDevice().bindBufferMemory(newBuffer, newBufferMemory, 0);
	//vkBindBufferMemory(device, newBuffer, newBufferMemory, 0);
	{
		vk::BufferCopy copyRegion = {};
		copyRegion.size = Size();
		SingleUseCommandBuffer cmdBuffer(cmdPool);
		cmdBuffer->copyBuffer(buffer, newBuffer, { copyRegion });
	}

	GetvkDevice().destroyBuffer(buffer);
	GetvkDevice().freeMemory(bufferMemory);
	buffer = newBuffer;
	bufferMemory = newBufferMemory;
	properties.size = newsize;

}

void Buffer::InsertData(uint32_t Offset, uint32_t size, void* data)
{
	auto dataMapped = reinterpret_cast<char*>(MapMemory());
	//std::copy(data,data+size,dataMapped);
	memcpy(dataMapped + Offset, data, size);
	UnmapMemory();
}

void Buffer::Destroy()
{
	GetDevice()->destroyBuffer(buffer);
	GetDevice()->freeMemory(bufferMemory);
}

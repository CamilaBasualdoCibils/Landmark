#include "pch.h"
#include "Buffer.h"


#include "G_API/Devices/LogicalDevice.h"
#include <G_API/Operations/SingleUseCommandBuffer.h>
Buffer::Buffer(size_t size, Flags<BufferUsage> usage, Flags<MemoryProperties> memoryProperties, SharingState sharingMode) :DeviceResource(), bufferUsage(usage), sharingState(sharingMode), BufferSize(size), memoryProerties(memoryProperties)
{


	BufferSize = size;
	vk::BufferCreateInfo buffer_create_info{};
	buffer_create_info.usage = usage;
	buffer_create_info.size = BufferSize;
	if (sharingState.shareMode == SharingMode::CONCURRENT && sharingState.QueueFamilies.empty())
		logger.Error("Buffer created with sharing state Concurrent must specify Queue Families to allow access");
	buffer_create_info.sharingMode = static_cast<vk::SharingMode>(sharingState.shareMode);
	buffer_create_info.queueFamilyIndexCount = sharingState.QueueFamilies.size();
	buffer_create_info.pQueueFamilyIndices = sharingState.QueueFamilies.data();

	_buffer = GetvkDevice().createBuffer(buffer_create_info).value;

	auto memoryRequirements = GetvkDevice().getBufferMemoryRequirements(_buffer);





	memoryTypeIndex = GetDevice().GetMemoryTypeThatFits(memoryRequirements.memoryTypeBits, memoryProerties);

	if (!memoryTypeIndex.isValid())
	{
		logger.Error("Buffer Memory type requirements could not be fullfilled ");
	}

	vk::MemoryAllocateInfo memory_allocate_info{};
	memory_allocate_info.allocationSize = memoryRequirements.size;
	memory_allocate_info.memoryTypeIndex = memoryTypeIndex.index;
	_bufferMemory = GetvkDevice().allocateMemory(memory_allocate_info).value;

	auto result = GetvkDevice().bindBufferMemory(_buffer, _bufferMemory, 0);

	std::stringstream ss;
	ss << "Buffer Allocated:\n"
		<< "	Size: " << BufferSize << "\n"
		<< "	Usage: " << bufferUsage << "\n"
		<< "	Sharing Mode: " << vk::to_string(static_cast<vk::SharingMode>(sharingState.shareMode)) << "\n"
		<< "	Memory Type: " << memoryProperties
		;
	logger.Debug(ss);

}

Buffer::~Buffer()
{

	GetvkDevice().destroyBuffer(_buffer);
	GetvkDevice().freeMemory(_bufferMemory);
	logger.Debug( "Buffer Unallocated");
}

void* Buffer::MapMemory()
{
	return GetvkDevice().mapMemory(_bufferMemory, 0, BufferSize).value;
}

void Buffer::UnmapMemory()
{
	GetvkDevice().unmapMemory(_bufferMemory);
}

size_t Buffer::Size()
{
	return BufferSize;
}

void Buffer::Realloc(CommandPool& cmdPool, const size_t newsize)
{
	if (!(bufferUsage & (BufferUsage::TRANSFER_DST | BufferUsage::TRANSFER_SRC)))
	{
		std::stringstream ss;
		ss << "Buffer Realloc is invalid. BufferUsage does not contain TRANSFER_DST & TRANSFER_SRC\n"
			<< "Buffer Usage: " << bufferUsage;
		logger.Error(ss.str());
		return;
	}
	vk::BufferCreateInfo bufferCreateInfo = {};
	//VkBufferCreateInfo bufferCreateInfo = {};
	bufferCreateInfo.sType = vk::StructureType::eBufferCreateInfo;
	bufferCreateInfo.size = newsize;  // Set the new size here
	bufferCreateInfo.usage = bufferUsage; // Adjust usage as needed


	//VkBuffer newBuffer;
	vk::Buffer newBuffer = GetvkDevice().createBuffer(bufferCreateInfo).value;
	//vkCreateBuffer(device, &bufferCreateInfo, nullptr, &newBuffer);


	vk::MemoryRequirements memRequirements = GetvkDevice().getBufferMemoryRequirements(newBuffer);
	//vkGetBufferMemoryRequirements(device, newBuffer, &memRequirements);

	vk::MemoryAllocateInfo allocInfo = {};
	allocInfo.sType = vk::StructureType::eMemoryAllocateInfo;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = memoryTypeIndex.index;

	vk::DeviceMemory newBufferMemory = GetvkDevice().allocateMemory(allocInfo).value;
	//vkAllocateMemory(device, &allocInfo, nullptr, &newBufferMemory);

	auto result = GetvkDevice().bindBufferMemory(newBuffer, newBufferMemory, 0);
	//vkBindBufferMemory(device, newBuffer, newBufferMemory, 0);
	{
		vk::BufferCopy copyRegion = {};
		copyRegion.size = Size();
		SingleUseCommandBuffer cmdBuffer(cmdPool);
		cmdBuffer->copyBuffer(_buffer, newBuffer, { copyRegion });
	}

	GetvkDevice().destroyBuffer(_buffer);
	GetvkDevice().freeMemory(_bufferMemory);
	_buffer = newBuffer;
	_bufferMemory = newBufferMemory;
	BufferSize = newsize;

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
	GetDevice()->destroyBuffer(_buffer);
	GetDevice()->freeMemory(_bufferMemory);
}

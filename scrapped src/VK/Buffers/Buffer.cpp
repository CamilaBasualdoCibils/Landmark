#include "pch.h"
#include "Buffer.h"

#include "VK/Devices/LogicalDevice.h"
#include <VK/Operations/SingleUseCommandBuffer.h>
#include <VK/Operations/CommandBuffer.h>
#include <misc/string_utils.h>
#include <VK/misc/VulkanStringTools.h>
Buffer::Buffer(const BufferProperties &_prop) : DeviceResource(), properties(_prop)
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
	VkBuffer a;
	ss << "Buffer Allocated: " << getVulkanHandle_Str(buffer) << "\n"
	   << "	Size: " << string_formatBytes(properties.size) << "\n"
	   << "	Usage: " << properties.usage_flags << "\n"
	   << "	Sharing Mode: " << vk::to_string(static_cast<vk::SharingMode>(properties.sharing_mode.share_mode)) << "\n"
	   << "	Memory Type: " << properties.memory_properties;
	// logger.Debug(ss);
}

Buffer::~Buffer()
{
}

void *Buffer::MapMemory()
{
	return GetvkDevice().mapMemory(bufferMemory, 0, properties.size).value;
}

void Buffer::UnmapMemory()
{
	GetvkDevice().unmapMemory(bufferMemory);
}

size_t Buffer::Size() const
{
	return properties.size;
}

void Buffer::Realloc_Copy(CommandPool &pool, const size_t newsize)
{
	if (!(properties.usage_flags & (BufferUsage::TRANSFER_DST | BufferUsage::TRANSFER_SRC)))
	{
		std::stringstream ss;
		ss << "Buffer Realloc is invalid. usage flags does not contain TRANSFER_DST & TRANSFER_SRC\n"
		   << "Buffer Usage: " << properties.usage_flags;
		logger.Error(ss.str());
		return;
	}
	auto new_buffer = MakeBuffer(properties, newsize);

	// vkBindBufferMemory(device, newBuffer, newBufferMemory, 0);
	{
		SingleUseCommandBuffer cmd_buffer(pool);
		vk::BufferCopy copyRegion = {};
		copyRegion.size = Size();

		cmd_buffer->copyBuffer(buffer, new_buffer.first, {copyRegion});
	}

	GetvkDevice().destroyBuffer(buffer);
	GetvkDevice().freeMemory(bufferMemory);

	properties.size = newsize;
	buffer = new_buffer.first;
	bufferMemory = new_buffer.second;
}

void Buffer::Realloc_NoCopy(const size_t newsize)
{
	auto new_buffer = MakeBuffer(properties, newsize);
	GetvkDevice().destroyBuffer(buffer);
	GetvkDevice().freeMemory(bufferMemory);

	properties.size = newsize;
	buffer = new_buffer.first;
	bufferMemory = new_buffer.second;
}

void Buffer::InsertData(uint32_t Offset, uint32_t size, void *data)
{
	LASSERT(Offset <= properties.size - size, "write would exceed bounds");
	auto dataMapped = reinterpret_cast<char *>(MapMemory());
	// std::copy(data,data+size,dataMapped);
	memcpy(dataMapped + Offset, data, size);
	UnmapMemory();
}

void Buffer::Destroy()
{
	GetvkDevice().destroyBuffer(buffer);
	GetvkDevice().freeMemory(bufferMemory);

	// logger.Debug( "Buffer Deallocated. " + getVulkanHandle_Str(buffer));
}

std::pair<vk::Buffer, vk::DeviceMemory> Buffer::MakeBuffer(const BufferProperties &prop, size_t size)
{

	vk::BufferCreateInfo bufferCreateInfo = {};
	// VkBufferCreateInfo bufferCreateInfo = {};
	bufferCreateInfo.size = size;			   // Set the new size here
	bufferCreateInfo.usage = prop.usage_flags; // Adjust usage as needed

	// VkBuffer newBuffer;
	vk::Buffer newBuffer = GetvkDevice().createBuffer(bufferCreateInfo).value;
	// vkCreateBuffer(device, &bufferCreateInfo, nullptr, &newBuffer);

	vk::MemoryRequirements memRequirements = GetvkDevice().getBufferMemoryRequirements(newBuffer);
	// vkGetBufferMemoryRequirements(device, newBuffer, &memRequirements);

	vk::MemoryAllocateInfo allocInfo = {};
	allocInfo.sType = vk::StructureType::eMemoryAllocateInfo;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = memory_type_index.index;

	vk::DeviceMemory newBufferMemory = GetvkDevice().allocateMemory(allocInfo).value;
	// vkAllocateMemory(device, &allocInfo, nullptr, &newBufferMemory);

	auto result = GetvkDevice().bindBufferMemory(newBuffer, newBufferMemory, 0);
	return {newBuffer, newBufferMemory};
}

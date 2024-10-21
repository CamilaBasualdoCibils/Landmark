#pragma once
#include "GenericEnums.h"
#include "Debug/Debug.h"
#include "VK/Devices/DeviceResource.h"
#include "VK/Devices/PhysicalDevice.h"
#include <VK/Operations/CommandPool.h>

class Buffer : public DeviceResource
{
public:
	struct SharingState
	{
		SharingMode share_mode;
		const std::vector<uint32_t> queue_families;
	};
	struct BufferProperties {
		size_t size = 0;
		Flags<BufferUsage> usage_flags = BufferUsage::TRANSFER_SRC | BufferUsage::TRANSFER_DST;
		Flags<MemoryProperties> memory_properties = MemoryProperties::HOST_COHERENT | MemoryProperties::HOST_VISIBLE;
		SharingState sharing_mode = { SharingMode::EXCLUSIVE };
	};
protected:


	vk::Buffer buffer;
	vk::DeviceMemory bufferMemory;
	
	BufferProperties properties;

	PhysicalDevice::MemoryTypeIndexInfo memory_type_index;

public:

	Buffer(const BufferProperties& _prop);
	~Buffer();
	operator vk::Buffer() {return buffer;}
	void* MapMemory();
	void UnmapMemory();
	size_t Size();
	void Realloc(CommandPool& queue, const size_t newsize);
	void InsertData(uint32_t Offset, uint32_t size, void* data);

	void Destroy() override;
};



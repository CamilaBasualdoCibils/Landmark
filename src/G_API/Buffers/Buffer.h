#pragma once
#include "G_API/GenericEnums.h"
#include "Debug/Debug.h"
#include "G_API/Devices/DeviceResource.h"
#include "G_API/Devices/PhysicalDevice.h"
#include <G_API/Operations/CommandPool.h>

class Buffer final : public DeviceResource
{
public:
	struct SharingState
	{
		SharingMode shareMode;
		const std::vector<uint32_t> QueueFamilies;
	};
protected:


	vk::Buffer _buffer;
	vk::DeviceMemory _bufferMemory;
	size_t BufferSize;

	const SharingState sharingState;
	Flags<BufferUsage> bufferUsage;
	const Flags<MemoryProperties> memoryProerties;

	PhysicalDevice::MemoryTypeIndexInfo memoryTypeIndex;

public:

	Buffer(size_t size, Flags<BufferUsage> usage = BufferUsage::TRANSFER_SRC | BufferUsage::TRANSFER_DST, Flags<MemoryProperties> memoryProperties = MemoryProperties::HOST_COHERENT | MemoryProperties::HOST_VISIBLE, SharingState sharingMode = { SharingMode::EXCLUSIVE });
	~Buffer();

	void* MapMemory();
	void UnmapMemory();
	size_t Size();
	void Realloc(CommandPool& queue, const size_t newsize);
	void InsertData(uint32_t Offset, uint32_t size, void* data);

	void Destroy() override;
};



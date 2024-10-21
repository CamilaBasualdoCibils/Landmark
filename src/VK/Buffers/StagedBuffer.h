#include <pch.h>
#include "Buffer.h"
#include <VK/Operations/CommandPool.h>
class StagedBuffer : DeviceResource
{

public:
    struct StagedBufferProperties : Buffer::BufferProperties
    {

        StagedBufferProperties()
        {
            memory_properties = MemoryProperties::DEVICE_LOCAL;
            usage_flags = BufferUsage::TRANSFER_DST;
        }
    };

private:
    Buffer buffer;
    std::optional<Buffer> staging_buffer;
    StagedBufferProperties properties;

public:
    StagedBuffer(const StagedBufferProperties &_prop) : buffer(_prop), properties(_prop)
    {
    }

    void *MapMemory();
    void UnmapMemory();
    void UnmapMemoryAndTransfer(CommandPool &pool);
    void Transfer(CommandPool &pool);
    void DisposeStagingBuffer();

private:
    Buffer &MakeOrGetStagingBuffer();
};
#pragma once
#include <VK/Operations/CommandBuffer.h>

#include "Buffer.h"
class StagedBuffer : public Buffer
{

public:
    struct StagedBufferProperties : Buffer::BufferProperties
    {

        StagedBufferProperties(size_t _size,Flags<BufferUsage> _buffer_usage, 
        Flags<MemoryProperties> _memory_properties = MemoryProperties::DEVICE_LOCAL)
        {
            size = _size;
            usage_flags = _buffer_usage;
            memory_properties = _memory_properties;
        }
    };
private:
    StagedBufferProperties properties;
protected:
    std::optional<Buffer> staging_buffer;

public:
    StagedBuffer(const StagedBufferProperties &_prop);

    void Realloc_NoCopy(const size_t newsize) override;
    void Realloc_Copy(CommandPool& pool, const size_t newsize) override;

    void Transfer(CommandBuffer& cmd_buffer);
    void DisposeStagingBuffer();
    void Destroy() override;
    void InsertData(uint32_t Offset, uint32_t size, void* data) override;
private:
    Buffer &MakeOrGetStagingBuffer();
};
#include "StagedBuffer.h"
#include <VK/Operations/SingleUseCommandBuffer.h>
void *StagedBuffer::MapMemory()
{
    return MakeOrGetStagingBuffer().MapMemory();
}

void StagedBuffer::UnmapMemoryAndTransfer(CommandPool &pool)
{
    UnmapMemory();
    Transfer(pool);
}
void StagedBuffer::UnmapMemory()
{
    LASSERT(staging_buffer.has_value(), "Unmap memory but staging buffer doesnt exist??");
    staging_buffer->UnmapMemory();
}
void StagedBuffer::Transfer(CommandPool &pool)
{
    LASSERT(staging_buffer.has_value(), "Transfer but staging buffer doesnt exist??");

    vk::BufferCopy copyRegion{};
    copyRegion.srcOffset = 0; // Optional
    copyRegion.dstOffset = 0; // Optional
    copyRegion.size = properties.size;
    SingleUseCommandBuffer cmd_buffer(pool);
    cmd_buffer->copyBuffer(staging_buffer.value(), buffer, {copyRegion});
}

void StagedBuffer::DisposeStagingBuffer()
{
    if (staging_buffer.has_value()) staging_buffer->Destroy();
    staging_buffer.reset();
}

Buffer &StagedBuffer::MakeOrGetStagingBuffer()
{
    if (staging_buffer.has_value())
        return staging_buffer.value();
    static Buffer::BufferProperties stage_buffer_properties = [this]()
    {
        Buffer::BufferProperties b;
        b.size = properties.size;
        b.usage_flags = BufferUsage::TRANSFER_SRC;
        b.memory_properties = MemoryProperties::HOST_COHERENT | MemoryProperties::HOST_VISIBLE;
        return b;
    }();

    staging_buffer.emplace(stage_buffer_properties);
    return staging_buffer.value();

}



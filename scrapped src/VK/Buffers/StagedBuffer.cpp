#include "StagedBuffer.h"
#include <VK/Operations/SingleUseCommandBuffer.h>
StagedBuffer::StagedBuffer(const StagedBufferProperties &_prop) : Buffer([_prop]() {
    Buffer::BufferProperties buffer_prop = _prop;
      buffer_prop.usage_flags |= BufferUsage::TRANSFER_DST ;
      buffer_prop.usage_flags |= BufferUsage::TRANSFER_SRC;
      return buffer_prop;
}()),properties(_prop)
    {
        properties.usage_flags |= BufferUsage::TRANSFER_DST;
      properties.usage_flags |= BufferUsage::TRANSFER_SRC;

    }

    void StagedBuffer::Realloc_NoCopy(const size_t newsize)
    {
        Buffer::Realloc_NoCopy(newsize);
        MakeOrGetStagingBuffer().Realloc_NoCopy(newsize);
    }

    void StagedBuffer::Realloc_Copy(CommandPool &pool, const size_t newsize)
    {
         Buffer::Realloc_Copy(pool,newsize);
        MakeOrGetStagingBuffer().Realloc_Copy(pool,newsize);
    }


void StagedBuffer::Transfer(CommandBuffer& cmd_buffer)
{
    LASSERT(staging_buffer.has_value(), "Transfer but staging buffer doesnt exist??");
    LASSERT(cmd_buffer.HasBegun(),"Command buffer has not Begun() for transfer");
    if (staging_buffer->Size() != Buffer::Size()) {
        Buffer::Realloc_NoCopy(staging_buffer->Size());
    }
    vk::BufferCopy copyRegion{};
    copyRegion.srcOffset = 0; // Optional
    copyRegion.dstOffset = 0; // Optional
    copyRegion.size = properties.size;

    cmd_buffer.CopyBuffer(*staging_buffer,*this,properties.size,0,0);

}

void StagedBuffer::DisposeStagingBuffer()
{
    if (staging_buffer.has_value()) staging_buffer->Destroy();
    staging_buffer.reset();
}

void StagedBuffer::Destroy()
{
   Buffer::Destroy();
    if (staging_buffer.has_value()) staging_buffer->Destroy();
    staging_buffer.reset();
}

void StagedBuffer::InsertData(uint32_t Offset, uint32_t size, void *data)
{
    MakeOrGetStagingBuffer().InsertData(Offset,size,data);
}

Buffer &StagedBuffer::MakeOrGetStagingBuffer()
{
    if (staging_buffer.has_value())
        return staging_buffer.value();

        Buffer::BufferProperties b;
        b.size = properties.size;
        b.usage_flags = {BufferUsage::TRANSFER_SRC,BufferUsage::TRANSFER_DST};
        b.memory_properties = MemoryProperties::HOST_COHERENT | MemoryProperties::HOST_VISIBLE;



    staging_buffer.emplace(b);
    return staging_buffer.value();

}



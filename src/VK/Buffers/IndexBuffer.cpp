#include "pch.h"
#include "IndexBuffer.h"


IndexBuffer::IndexBuffer(size_t size):StagedBuffer(StagedBuffer::StagedBufferProperties{size,BufferUsage::INDEX_BUFFER})
{
			
}

void IndexBuffer::Bind(CommandBuffer& cmdBuffer)
{
	cmdBuffer->bindIndexBuffer(*this,0,vk::IndexType::eUint32);
}

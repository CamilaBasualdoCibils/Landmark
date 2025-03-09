#include "pch.h"
#include "VertexBuffer.h"


VertexBuffer::VertexBuffer(size_t size) :
	StagedBuffer(StagedBuffer::StagedBufferProperties{size,BufferUsage::VERTEX_BUFFER})
{

}

void VertexBuffer::Bind(CommandBuffer& cmdbuffer)
{
	vk::DeviceSize offset = 0;
	cmdbuffer->bindVertexBuffers(0,{*this},{offset});
}


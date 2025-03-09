#include "pch.h"
#include "UniformBuffer.h"

UniformBuffer::UniformBuffer(size_t size):
        StagedBuffer(StagedBuffer::StagedBufferProperties{size,BufferUsage::UNIFORM_BUFFER}) {
}


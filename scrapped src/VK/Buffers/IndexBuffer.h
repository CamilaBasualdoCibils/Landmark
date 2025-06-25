#pragma once
#include "StagedBuffer.h"
#include <VK/Operations/CommandBuffer.h>

class  IndexBuffer : public StagedBuffer
{
public:
	IndexBuffer(size_t size);

	void Bind(CommandBuffer& cmdBuffer);
};





#pragma once
#include "StagedBuffer.h"
#include <VK/Operations/CommandBuffer.h>
class  VertexBuffer : public StagedBuffer
{
public:

	VertexBuffer(size_t size);


	void Bind(CommandBuffer& cmdbuffer);
};


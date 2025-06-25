#include "pch.h"
#include "SingleUseCommandBuffer.h"

SingleUseCommandBuffer::SingleUseCommandBuffer(CommandPool& pool) :cmdBuffer(pool)
{


	cmdBuffer.Begin(CommandBufferUsageFlags::ONE_TIME_SUBMIT);
}

SingleUseCommandBuffer::~SingleUseCommandBuffer()
{
	cmdBuffer.End();
	cmdBuffer.SubmitAndWait();
	cmdBuffer.Destroy();

}



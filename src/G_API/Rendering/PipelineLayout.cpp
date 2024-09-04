#include "pch.h"
#include "PipelineLayout.h"

PipelineLayout::PipelineLayout()
{
	vk::PipelineLayoutCreateInfo create_info;
	
	layout = GetvkDevice().createPipelineLayout(create_info).value;
	logger.Debug("Pipeline Layout Created");

}

void PipelineLayout::Destroy()
{
	GetvkDevice().destroy(layout);
}

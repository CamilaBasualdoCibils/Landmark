#include "pch.h"
#include "PipelineLayout.h"
#include "PipelineLayout.hpp"
/*
PipelineLayout::PipelineLayout(std::vector<DescriptorSetLayout> descriptor_set_layouts )
{
	vk::PipelineLayoutCreateInfo create_info;
	std::vector<vk::DescriptorSetLayout> vk_dsl;
	for (uint32_t i = 0; i < descriptor_set_layouts.size(); i++) {
		vk_dsl.push_back(descriptor_set_layouts[i]);
	}


	vk::PushConstantRange push_constant_range;
	create_info.setSetLayouts(vk_dsl);
	layout = GetvkDevice().createPipelineLayout(create_info).value;
	logger.Debug("Pipeline Layout Created");

}*/

PipelineLayout::PipelineLayout(const Pipeline_layout_properties & prop) :m_properties(prop){

	vk::PipelineLayoutCreateInfo create_info;
	std::vector<vk::DescriptorSetLayout> vk_dsl(prop.descriptor_set_layouts.size());
	std::copy(prop.descriptor_set_layouts.begin(),prop.descriptor_set_layouts.end(),vk_dsl.begin());
	create_info.setSetLayouts(vk_dsl);
	layout = GetvkDevice().createPipelineLayout(create_info).value;
}

void PipelineLayout::Destroy()
{
	GetvkDevice().destroy(layout);
}

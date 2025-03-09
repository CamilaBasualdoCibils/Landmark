#pragma once
#include "VK/Devices/DeviceResource.h"
#include <VK/Descriptors/DescriptorSetLayout.h>
class  PipelineLayout final : public DeviceResource
{
	public:
	struct Pipeline_layout_properties {
		std::vector<DescriptorSetLayout> descriptor_set_layouts = {};
	};
	private:
	Pipeline_layout_properties m_properties;
	vk::PipelineLayout layout;


public:
	//PipelineLayout(std::vector<DescriptorSetLayout> descriptor_set_layouts ={});
	PipelineLayout(const Pipeline_layout_properties&);
	void Destroy() override;
	operator vk::PipelineLayout() const {return layout;}

	const vk::PipelineLayout GetLayout() const { return layout; }
};

#pragma once
#include "G_API/Devices/DeviceResource.h"

class  PipelineLayout final : public DeviceResource
{
	vk::PipelineLayout layout;


public:
	PipelineLayout(); //TODO

	void Destroy() override;

	const vk::PipelineLayout GetLayout() const { return layout; }
};

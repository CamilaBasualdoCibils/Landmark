#pragma once
#include "PipelineLayout.h"
#include "RenderPass.h"
#include "GenericEnums.h"
#include "VK/Devices/DeviceResource.h"
class Pipeline final : public DeviceResource
{
public:
	struct PipelineRenderProperties
	{
		std::vector<DynamicStates> dynamicStates = {DynamicStates::VIEWPORT, DynamicStates::SCISSOR};
		PrimitiveTopologies topology = PrimitiveTopologies::TRIANGLE;

		// viewport & Scissor
		glm::vec2 ViewportMin = {0, 0};
		glm::vec2 ViewportSize = {512, 512};
		glm::ivec2 ScissorMin = {0, 0};
		glm::uvec2 ScissorSize = ViewportSize;

		// Rasterizer
		bool DepthClamp = false;
		bool DiscardEnable = false;
		PolygonModes polygonMode = PolygonModes::FILL;
		float LineWidth = 1;
		CullModes cullMode = CullModes::BACK;

		bool BlendEnable = false;
		Flags<ColorComponents> colorWriteMask = {ColorComponents::RED, ColorComponents::BLUE, ColorComponents::GREEN, ColorComponents::ALPHA};
	};
	struct PipelineProperties
	{

		std::map<ShaderStage, std::vector<char>> shaderStages;
		PipelineRenderProperties renderProperties;
	};

private:
	vk::Pipeline pipeline;
	// const vk::PipelineLayout layout;

	PipelineProperties info;

public:
	Pipeline(const PipelineProperties &_info, const PipelineLayout &_layout, const RenderPass &rp, uint32_t subpass = 0);
	void Destroy() override;

	vk::Pipeline GetVkPipeline() const {return pipeline;}
	constexpr operator vk::Pipeline() const {return pipeline;}
};

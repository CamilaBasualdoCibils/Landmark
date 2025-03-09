#pragma once
#include "PipelineLayout.h"
#include "RenderPass.h"
#include "GenericEnums.h"
#include "VK/Devices/DeviceResource.h"
class Pipeline final : public DeviceResource
{
public:

	struct VertexBinding {
		uint32_t binding = 0; //Id of array. usually always zero unless multiple vertex buffers are supplied
		size_t stride = 0; //size of the vertex
		VertexInputRate input_rate = VertexInputRate::VERTEX;
	};
	struct VertexAttribute {
		uint32_t binding = 0;// From which VertexBinding this data comes from
		uint32_t location = 0; // equates to the location parameter in the shader
		Format format = Format::RGBA_32_SFLOAT;
		uint32_t offset = 0; //
	};
	struct Pipeline_Render_Properties
	{
		std::vector<DynamicStates> dynamicStates = {DynamicStates::VIEWPORT, DynamicStates::SCISSOR};
		PrimitiveTopologies topology = PrimitiveTopologies::TRIANGLE;
		std::vector<VertexBinding> vertex_bindings;
		std::vector<VertexAttribute> vertex_attributes;
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
		Pipeline_Render_Properties renderProperties;
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

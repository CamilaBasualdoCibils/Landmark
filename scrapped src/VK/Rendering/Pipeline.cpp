#include "pch.h"
#include "Pipeline.h"
#include "Pipeline.hpp"

Pipeline::Pipeline(const PipelineProperties& _info, const PipelineLayout& _layout, const RenderPass& rp, uint32_t subpass):info(_info)
{
	vk::PipelineDynamicStateCreateInfo dynamic_state_create;
		std::vector<vk::DynamicState> dynamic_states;
	{
		//logger.Warning("Fucking idiot didnt implement dynamic states. tomorrows problems are tomorrow's me");

		for (auto& ds : info.renderProperties.dynamicStates) {
			vk::DynamicState d =enum_cast( ds);
			dynamic_states.push_back(d);
		}
			
		dynamic_state_create.dynamicStateCount = dynamic_states.size();
		dynamic_state_create.pDynamicStates = dynamic_states.data();

	}
	std::vector<vk::VertexInputAttributeDescription> vk_v_attribs;
	for (const auto& attrib: info.renderProperties.vertex_attributes) {
		vk::VertexInputAttributeDescription attrib_description;
		attrib_description.binding = attrib.binding;
		attrib_description.location = attrib.location;
		attrib_description.format = (vk::Format)attrib.format;
		attrib_description.offset = attrib.offset;
		vk_v_attribs.push_back(attrib_description);
	}

	std::vector<vk::VertexInputBindingDescription> vk_v_binding;
	for (const auto& binding: info.renderProperties.vertex_bindings) {
		vk::VertexInputBindingDescription binding_description;
		binding_description.binding = binding.binding;
		binding_description.stride = binding.stride;
		binding_description.inputRate = enum_cast(binding.input_rate);
		vk_v_binding.push_back(binding_description);

	}
	vk::PipelineVertexInputStateCreateInfo vertex_input_state_create;
	vertex_input_state_create.setVertexAttributeDescriptions(vk_v_attribs);
	vertex_input_state_create.setVertexBindingDescriptions(vk_v_binding);
	vk::PipelineInputAssemblyStateCreateInfo input_assembly_state_create;
	{
		input_assembly_state_create.topology = vk::PrimitiveTopology(info.renderProperties.topology);
	}
	


	vk::Viewport viewport{ info.renderProperties.ViewportMin.x,info.renderProperties.ViewportMin.y,
	info.renderProperties.ViewportSize.x,info.renderProperties.ViewportSize.y,0,1 };
	vk::Rect2D scissor{ {info.renderProperties.ScissorMin.x,info.renderProperties.ScissorMin.y},
	{info.renderProperties.ScissorSize.x,info.renderProperties.ScissorSize.y} };

	vk::PipelineViewportStateCreateInfo viewport_state_create_info;
	{
		viewport_state_create_info.viewportCount = 1;
		viewport_state_create_info.scissorCount = 1;
		viewport_state_create_info.pViewports = &viewport;
		viewport_state_create_info.pScissors = &scissor;
	}
	

	vk::PipelineRasterizationStateCreateInfo rasterization_state_create;
	{
		rasterization_state_create.depthClampEnable = info.renderProperties.DepthClamp;
		rasterization_state_create.rasterizerDiscardEnable = info.renderProperties.DiscardEnable;
		rasterization_state_create.polygonMode = vk::PolygonMode(info.renderProperties.polygonMode);
		rasterization_state_create.lineWidth = info.renderProperties.LineWidth;
		rasterization_state_create.cullMode = vk::CullModeFlagBits(info.renderProperties.cullMode);
		rasterization_state_create.frontFace = vk::FrontFace::eCounterClockwise;
	}

	vk::PipelineMultisampleStateCreateInfo multisample_state_create;
	{
		multisample_state_create.sampleShadingEnable = false;
		multisample_state_create.rasterizationSamples = vk::SampleCountFlagBits(SampleCountFlags::e1);

	}

	vk::PipelineColorBlendAttachmentState colorBlendAttachment;
	{
		colorBlendAttachment.colorWriteMask = info.renderProperties.colorWriteMask;
		colorBlendAttachment.blendEnable = info.renderProperties.BlendEnable;
		//TODO: THE REST
	}
	vk::PipelineColorBlendStateCreateInfo colorBlending;
	{
		colorBlending.logicOpEnable = false;
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &colorBlendAttachment;
	}

	std::vector<vk::ShaderModule> modules;
	
	std::vector<vk::PipelineShaderStageCreateInfo> shaderStagesInfos;
	{
		modules.reserve(info.shaderStages.size());
		shaderStagesInfos.reserve(info.shaderStages.size());
	}
	for (auto& stage : info.shaderStages)
	{
		vk::ShaderModuleCreateInfo shader_module_create;
		shader_module_create.codeSize = stage.second.size() * sizeof(stage.second[0]);
		shader_module_create.pCode = reinterpret_cast<const uint32_t*>( stage.second.data());
		modules.push_back( GetvkDevice().createShaderModule(shader_module_create).value);

		vk::PipelineShaderStageCreateInfo create_info;
		create_info.pName = "main";
	

		create_info.module = modules.back();
		create_info.stage = vk::ShaderStageFlagBits( stage.first);
		shaderStagesInfos.push_back(create_info);
	}

	vk::GraphicsPipelineCreateInfo pipeline_info;
	pipeline_info.stageCount = modules.size();
	pipeline_info.pStages = shaderStagesInfos.data();
	pipeline_info.pVertexInputState = &vertex_input_state_create;
	pipeline_info.pInputAssemblyState = &input_assembly_state_create;
	pipeline_info.pViewportState = &viewport_state_create_info;
	pipeline_info.pRasterizationState = &rasterization_state_create;
	pipeline_info.pMultisampleState = &multisample_state_create;
	pipeline_info.pDepthStencilState = nullptr;
	pipeline_info.pColorBlendState = &colorBlending;
	pipeline_info.pDynamicState = &dynamic_state_create;
	pipeline_info.layout = _layout.GetLayout();
	pipeline_info.renderPass = rp.GetVkRenderPass();

	pipeline_info.subpass = subpass;

	auto result = GetvkDevice().createGraphicsPipeline(VK_NULL_HANDLE, pipeline_info);
	if (result.result != vk::Result::eSuccess)
	{
		logger.Error( "Failed to create Graphics Pipeline");
	}
	pipeline = result.value;
}

void Pipeline::Destroy()
{
	GetvkDevice().destroyPipeline(pipeline);
	
}


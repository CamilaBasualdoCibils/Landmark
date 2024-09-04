#include "pch.h"
#include "Pipeline.h"

Pipeline::Pipeline(const PipelineProperties& _info, const PipelineLayout& _layout, const RenderPass& rp, uint32_t subpass):info(_info)
{
	vk::PipelineDynamicStateCreateInfo dynamic_state_create;
	{
		logger.Warning("Fucking idiot didnt implement dynamic states. tomorrows problems are tomorrow's me");
		/*
		std::vector<vk::DynamicState> dynamic_states;
		for (auto& ds : info.renderProperties.dynamicStates) {
			vk::DynamicState d =EnumCast( ds);
			dynamic_states.push_back(d);
		}
			
		dynamic_state_create.dynamicStateCount = dynamic_states.size();
		dynamic_state_create.pDynamicStates = dynamic_states.data();
		*/
	}

	vk::PipelineVertexInputStateCreateInfo vertex_input_state_create;


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
		shader_module_create.codeSize = stage.second.size();
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

#pragma once
#include "IO/File.h"
#include "IO/IO.h"
//#include "IO/Resource.h"
//#include "Renderer/Renderer.h"
#include "VK/Rendering/Pipeline.h"
//#include "Vulkan/Rendering/GraphicsPipeline.h"

class  Material
{
	static inline std::optional<PipelineLayout> MaterialPipelineLayout;
	std::optional<Pipeline> pipeline;
	//Material(std::string source);
	//Material(IO::File& file);
	//GraphicsPipeline RasterShader;
	//GraphicsPipeline Ray_HitShader;

public:
	Material(const std::string& vert, const std::string& frag)
	{
		if (!MaterialPipelineLayout.has_value())
		{
			//MaterialPipelineLayout.emplace();
		}

		auto f = IO::GetResources().GetFolder("materials");
		Pipeline::PipelineProperties pipeline_construction_info;
		//pipeline_construction_info.shaderStages[ShaderStage::VERTEX] = f.GetFile(vert).GetContent_Binary();
		//pipeline_construction_info.shaderStages[ShaderStage::FRAGMENT] = f.GetFile(frag).GetContent_Binary();
		//pipeline.emplace(pipeline_construction_info, MaterialPipelineLayout.value(), Renderer::GetRenderPass());
	}
};


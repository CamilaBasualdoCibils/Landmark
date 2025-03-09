//
// Created by camila on 10/29/24.
//

#include "Lens.h"
#include "Stage.h"
#include "Act.hpp"

Lens::Lens(const lens_properties &internal, Stage &st,Act& act):properties(internal)
{


    Pipeline::PipelineProperties pl_prop;
    
    const VertexFormat &vertex_format = st.GetVertexFormat();
    const auto& vertex_format_pl = vertex_format.toPipelineVertexInput();
    pl_prop.renderProperties.vertex_attributes=vertex_format_pl.first;
    pl_prop.renderProperties.vertex_bindings = vertex_format_pl.second;

    pl_prop.shaderStages = internal.shaderStages;
    pl_prop.renderProperties.cullMode = internal.cull_mode;
    pipeline.emplace(pl_prop, st.getPipelineLayout(), act.getRenderPass(), 0);
}

Lens::~Lens()
{
 
   
    if (pipeline.has_value())pipeline->Destroy();
    logger.Debug("lens destroyed");
}

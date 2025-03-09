#include "structs.h"

std::pair<decltype(Pipeline::Pipeline_Render_Properties::vertex_attributes), decltype(Pipeline::Pipeline_Render_Properties::vertex_bindings)> VertexFormat::toPipelineVertexInput() const
{
    std::pair<decltype(Pipeline::Pipeline_Render_Properties::vertex_attributes), decltype(Pipeline::Pipeline_Render_Properties::vertex_bindings)> pair;
    auto &attrib_list = pair.first;
    auto &binding_list = pair.second;
    uint32_t AttributeLocation = 0;
    // each buffer
    for (int bufferID = 0; bufferID < buffer_inputs.size(); bufferID++)
    {
        const VertexFormat::Element_list &elements = buffer_inputs[bufferID];

        // each vertex bindings
        for (int InputBindingID = 0; InputBindingID < elements.size(); InputBindingID++)
        {
            Pipeline::VertexBinding binding;
            binding.input_rate = elements[InputBindingID].input_rate;
            binding.binding = bufferID;
            const VertexInputElement &element = elements[InputBindingID];
            size_t stride = 0;

            for (int Attribute_BindingID = 0; Attribute_BindingID < element.attributes.size(); Attribute_BindingID++)
            {
                Pipeline::VertexAttribute attribute;
                attribute.location = AttributeLocation++;
                attribute.binding = bufferID;
                attribute.offset = element.attributes[Attribute_BindingID].offset;
                attribute.format = (Format)element.attributes[Attribute_BindingID].type;
                size_t sizeof_attribute = StageInstanceDataTypeSize.at(element.attributes[Attribute_BindingID].type);

                stride += sizeof_attribute;
                attrib_list.push_back(attribute);
            }
            binding.stride = stride;
            binding.input_rate = element.input_rate;
            binding_list.push_back(binding);
        }
    }
    return pair;
}

size_t stage_instance_parameter::totalSizeFromParameterList(const std::vector<stage_instance_parameter> &p)
{
    size_t s = 0;
    std::for_each(p.begin(), p.end(), [&](const stage_instance_parameter &v)
                  { s += StageInstanceDataTypeSize.at(v.type); });
    return s;
}
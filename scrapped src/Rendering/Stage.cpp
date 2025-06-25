//
// Created by camila on 10/29/24.
//

#include "Stage.h"
#include "Lens.h"
#include "Act.hpp"
Stage::Stage(const stage_properties &_prop) : properties(_prop),
                                              v_vertex_data(10240), v_actor_data(1024), v_index_data(10240)
{
    descriptor_set_layout.emplace(_prop.descriptor_set_layouts_bindings);
    DescriptorPool::DescriptorPoolProperties ds_pool_properties;
    ds_pool_properties._maxSets = 1;
    descriptor_pool.emplace(ds_pool_properties);
    descriptor_set.emplace(*descriptor_pool, *descriptor_set_layout);
    PipelineLayout::Pipeline_layout_properties pl_prop;

    pl_prop.descriptor_set_layouts = {descriptor_set_layout.value()};
    pipeline_layout.emplace(pl_prop);
}

Stage::~Stage()
{

    if (pipeline_layout.has_value())
        pipeline_layout->Destroy();
    if (descriptor_set_layout.has_value())
        descriptor_set_layout->Destroy();
    if (descriptor_set.has_value())
        descriptor_set->Destroy();
    if (descriptor_pool.has_value())
        descriptor_pool->Destroy();
    logger.Debug("Stage destroyed");
}

const PipelineLayout &Stage::getPipelineLayout()
{
    return pipeline_layout.value();
}


void Stage::Draw(CommandBuffer &cmd_buffer)
{
TransferData(cmd_buffer);
    cmd_buffer->bindVertexBuffers(0, {v_vertex_data, v_actor_data}, {0, 0});
    cmd_buffer->bindIndexBuffer(v_index_data, 0, vk::IndexType::eUint32);
    cmd_buffer.BindDescriptorSets(*pipeline_layout, {*descriptor_set}, 0, 0, PipelineBindPoint::GRAPHICS);

    size_t index_count = index_data.size() / sizeof(uint32_t);
    cmd_buffer.DrawIndexed(index_count);
}

void Stage::TransferData(CommandBuffer &cmd_buffer)
{
    auto check_and_transfer = [&cmd_buffer](bool &flag, std::vector<std::byte> data, StagedBuffer &target)
    { if (flag) {
        if (target.Size() < data.size()) 
            target.Realloc_NoCopy(data.size());

        target.InsertData(0,data.size(),data.data());
        target.Transfer(cmd_buffer);
        flag = false;
    } };

    check_and_transfer(vertex_dirty, vertex_data, v_vertex_data);
    check_and_transfer(actor_dirty, actor_data, v_actor_data);
    check_and_transfer(index_dirty, index_data, v_index_data);
}

VertexFormat Stage::GetVertexFormat() const
{
    VertexFormat vf;
    VertexFormat::Element_list vertex_data;
    VertexInputElement data_per_vertex;
    data_per_vertex.input_rate = VertexInputRate::VERTEX;
    size_t currentOffset = 0;
    std::for_each(properties.vertex_parameters.begin(), properties.vertex_parameters.end(), [&](const stage_instance_parameter &sip)
                  {
        VertexAttribute va;
        va.type = sip.type;
        va.offset = currentOffset;
        data_per_vertex.attributes.push_back(va);
        currentOffset += StageInstanceDataTypeSize.at(sip.type); });

    VertexInputElement data_per_actor;
    data_per_actor.input_rate = VertexInputRate::INSTANCE;
    currentOffset = 0;
    std::for_each(properties.actor_parameters.begin(), properties.actor_parameters.end(), [&](const stage_instance_parameter &sip)
                  {
        VertexAttribute va;
        va.type = sip.type;
        va.offset = currentOffset;
        data_per_actor.attributes.push_back(va);
        currentOffset += StageInstanceDataTypeSize.at(sip.type); });

    vf.buffer_inputs = {{data_per_vertex},{ data_per_actor}};
    return vf;
}

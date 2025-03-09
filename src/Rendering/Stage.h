//
// Created by camila on 10/29/24.
//
#pragma once
#ifndef STAGE_H
#define STAGE_H

#include <VK/Rendering/PipelineLayout.h>
#include "structs.h"
#include <VK/Descriptors/DescriptorSet.h>
#include <VK/Operations/CommandBuffer.h>
#include <VK/Buffers/VertexBuffer.h>
#include <VK/Buffers/IndexBuffer.h>
#include <Types/Viewport.h>
#include "VK/Operations/SingleUseCommandBuffer.h"
class Act;
class Lens;
class Frame;

class Stage
{
public:
    Stage(const stage_properties &_prop);
    ~Stage();
    const PipelineLayout &getPipelineLayout();
    struct CaptureProperties {
        std::vector<Viewport> viewports;
    };
    //void Capture(std::weak_ptr<Act> act,std::weak_ptr<Lens> lens, std::weak_ptr<Frame> frame,CommandBuffer& cmd_buffer,const CaptureProperties& cap_properties);
    void Draw(CommandBuffer& cmd_buffer);
    using buffer_data = std::vector<char>;
    
    struct DataExpectations {
        size_t VertexDataSize;
        size_t indexDataSize;
        size_t ActorDataSize;
    };
    template <typename T>
    void InsertVertexData(std::span<T> data, size_t offset = 0);
    template <typename T>
    void InsertActorData(std::span<T> data, size_t offset = 0);
    template <typename T>
    void InsertIndexData(std::span<T> data, size_t offset = 0);
    void TransferData(CommandBuffer& cmd_buffer);
    VertexFormat GetVertexFormat() const;
private:
template <typename T>
void InsertDataInto(std::vector<std::byte>& dest,std::span<T> span,size_t offset);
  
    stage_properties properties;
    std::optional<PipelineLayout> pipeline_layout;
    std::optional<DescriptorSetLayout> descriptor_set_layout;
    std::optional<DescriptorSet> descriptor_set;
    std::optional<DescriptorPool> descriptor_pool;

    VertexBuffer v_vertex_data;
    VertexBuffer v_actor_data;
    IndexBuffer v_index_data;
    bool vertex_dirty = false, actor_dirty = false, index_dirty = false;
    std::vector<std::byte> vertex_data,actor_data,index_data;


    static inline Log logger = Log("Stage");


};

#endif // STAGE_H

template <typename T>
inline void Stage::InsertVertexData(std::span<T> data, size_t offset)
{
InsertDataInto(vertex_data,data,offset);
vertex_dirty = true;
}

template <typename T>
inline void Stage::InsertActorData(std::span<T> data, size_t offset)
{
InsertDataInto(actor_data,data,offset);
actor_dirty = true;
}

template <typename T>
inline void Stage::InsertIndexData(std::span<T> data, size_t offset)
{
    InsertDataInto(index_data,data,offset);
    index_dirty = true;
}

template <typename T>
inline void Stage::InsertDataInto(std::vector<std::byte> &dest, std::span<T> span, size_t offset)
{
     std::span<std::byte> sp = std::span(reinterpret_cast<std::byte*>(span.data()),span.size_bytes());
    dest.insert(dest.begin() + offset,sp.begin(),sp.end());
}

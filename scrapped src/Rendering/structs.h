//
// Created by camila on 12/3/24.
//

#ifndef RENDERING_STRUCTS_H
#define RENDERING_STRUCTS_H
#include <pch.h>
#include <GenericEnums.h>
#include <VK/Rendering/Pipeline.h>
#include <VK/Rendering/RenderPass.h>
#include <VK/Operations/Queue.h>
#include <VK/Rendering/Swapchain.h>
enum class StageInstanceDataType
{
    // floats
    FLOAT = int_cast(Format::R_32_SFLOAT),
    FLOAT2 = int_cast(Format::RG_32_SFLOAT),
    FLOAT3 = int_cast(Format::RGB_32_SFLOAT),
    FLOAT4 = int_cast(Format::RGBA_32_SFLOAT),

    INT,
    INT2,
    INT3,
    INT4,
    UINT,
    UINT2,
    UINT3,
    UINT4,
    DOUBLE = int_cast(Format::R_64_SFLOAT),
    DOUBLE2,
    DOUBLE3,
    DOUBLE4,
};

const static std::unordered_map<StageInstanceDataType, size_t> StageInstanceDataTypeSize = {
    {
        StageInstanceDataType::FLOAT,
        sizeof(vec<1, float>),
    }, // FLOAT
    {
        StageInstanceDataType::FLOAT2,
        sizeof(vec<2, float>),
    }, // FLOAT2
    {
        StageInstanceDataType::FLOAT3,
        sizeof(vec<3, float>),
    }, // FLOAT3
    {
        StageInstanceDataType::FLOAT4,
        sizeof(vec<4, float>),
    }, // FLOAT4
    {
        StageInstanceDataType::INT,
        sizeof(vec<1, int32_t>),
    }, // INT
    {
        StageInstanceDataType::INT2,
        sizeof(vec<2, int32_t>),
    }, // INT2
    {
        StageInstanceDataType::INT3,
        sizeof(vec<3, int32_t>),
    }, // INT3
    {
        StageInstanceDataType::INT4,
        sizeof(vec<4, int32_t>),
    }, // INT4
    {
        StageInstanceDataType::UINT,
        sizeof(vec<1, uint32_t>),
    }, // UINT
    {
        StageInstanceDataType::UINT2,
        sizeof(vec<2, uint32_t>),
    }, // UINT2
    {
        StageInstanceDataType::UINT3,
        sizeof(vec<3, uint32_t>),
    }, // UINT3
    {
        StageInstanceDataType::UINT4,
        sizeof(vec<4, uint32_t>),
    }, // UINT4
    {
        StageInstanceDataType::DOUBLE,
        sizeof(vec<1, double>),
    }, // DOUBLE
    {
        StageInstanceDataType::DOUBLE2,
        sizeof(vec<2, double>),
    }, // DOUBLE2
    {
        StageInstanceDataType::DOUBLE3,
        sizeof(vec<3, double>),
    }, // DOUBLE3
    {
        StageInstanceDataType::DOUBLE4,
        sizeof(vec<4, double>),
    }, // DOUBLE4
};

struct VertexAttribute
{
    StageInstanceDataType type = StageInstanceDataType::FLOAT3;
    size_t offset = 0;
};
struct VertexInputElement
{
    std::vector<VertexAttribute> attributes; // every type in this element in order of location
    VertexInputRate input_rate = VertexInputRate::VERTEX;
};
struct VertexFormat
{
    using Element_list = std::vector<VertexInputElement>; // elements per buffer
    using Buffer_list = std::vector<Element_list>;        // buffer list

    Buffer_list buffer_inputs;

    std::pair<
        decltype(Pipeline::Pipeline_Render_Properties::vertex_attributes),
        decltype(Pipeline::Pipeline_Render_Properties::vertex_bindings)>
    toPipelineVertexInput() const;
};

struct schedule_properties
{
    Queue *rendering_queue;
    Queue *presention_queue;
};
struct schedule_capture_stage_properties {

};
struct stage_instance_parameter
{
    std::string name;
    StageInstanceDataType type;

    static size_t totalSizeFromParameterList(const std::vector<stage_instance_parameter> &p);
};

struct stage_properties
{
    std::vector<stage_instance_parameter> vertex_parameters;
    std::vector<stage_instance_parameter> actor_parameters;

    //std::vector<stage_parameter> global_parameters; //doesnt work since this could have custom structs and shit
    //VertexFormat vertexFormat;
    
    std::vector<DescriptorSetLayoutBinding> descriptor_set_layouts_bindings;
};
struct act_properties
{
    decltype(RenderPass::Renderpass_Properties::color_attachments) color_attachments;
};
struct lens_properties
{
    decltype(Pipeline::PipelineProperties::shaderStages) shaderStages;
    decltype(Pipeline::Pipeline_Render_Properties::cullMode) cull_mode = CullModes::BACK;
};
struct frame_properties
{
    uvec3 dimensions = {0, 0, 0};
    uint32_t lens_subpass_id = 0;
};
struct film_properties
{
    vk::SurfaceKHR surface;
    decltype(Swapchain::SwapchainProperties::presentMode) present_mode;
    // if none specified then Act's first attachment format will be used
    std::optional<Format> format;
    ColorSpace color_space = ColorSpace::SRGB_NON_LINEAR;
};

#endif // RENDERING_STRUCTS_H

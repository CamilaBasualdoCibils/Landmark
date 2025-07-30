#pragma once
#include "EditorWindowToolGeneric.hpp"
#include "Graphics/Pipelines/ShaderCompiler.hpp"
#include "Graphics/Vulkan/Pipeline/VKPipeline.hpp"
#include "misc/Singleton.hpp"
#include <pch.h>
namespace Editor
{
class TemplatesData : public Singleton<TemplatesData>
{

    GPURef<VK::Pipeline> ColorPickerPipeline;

  public:
    TemplatesData() {
        // GPURef<VK::PipelineLayout> EmptyPipelineLayout =
        //     GPURef<VK::PipelineLayout>::MakeRef(VK::PipelineLayoutProperties{});
        // const VK::PipelineProperties ColorPickerPipelineProperties{
        //     .Layout = EmptyPipelineLayout,
        //     .VariantProperties = VK::GraphicsPipelineProperties{
        //         .VertexBinary = Graphics::ShaderCompiler::Get().Compile(Graphics::ShaderCompileInfo{.}).data}};
        // ColorPickerPipeline.Make(ColorPickerPipelineProperties);
    };
};
bool DragVec3(const std::string &Label, vec3 *data, float v_speed = 1.0f);
bool Rotation(const std::string &Label, quat *data);
bool MenuItem(const std::string &Item, bool *selected, bool Enabled = true);

inline bool ColorPicker(const std::string &Label, vec4 *color)
{
    bool modified = ImGui::ColorPicker4(Label.c_str(), &color->x);
    ImGui::GetWindowDrawList()->AddCallback([](const ImDrawList *parent_list, const ImDrawCmd *cmd) {}, nullptr);
    return modified;
}
class TemplatesDemo : public WindowToolGeneric
{
    void WindowContents() override
    {
        static quat Rot;
        Rotation("Rotation", &Rot);

        static vec4 Color;
        ColorPicker("ColorPicker", &Color);
    }

  public:
    TemplatesDemo() : WindowToolGeneric("Template Demo")
    {
    }
};
} // namespace Editor

#pragma once
#include "CommandManager.hpp"

#include "Graphics/Vulkan/Buffer/Buffer.hpp"
#include <pch.h>
#include "Graphics/Vulkan/Instance.hpp"
namespace VK
{
struct InsertLabelCommand : Command
{
    std::string LabelName;
    vec4 Color;

    InsertLabelCommand(std::string LabelName, vec4 Color) : LabelName(std::move(LabelName)), Color(Color)
    {
    }

    std::string GetName() override
    {
        return "InsertLabel: " + LabelName;
    }

    void Execute(GPURef<VK::CommandBuffer> cmdBuffer) override
    {
        vk::DebugUtilsLabelEXT label;
        label.pLabelName = LabelName.c_str();
        label.color[0] = Color.r;
        label.color[1] = Color.g;
        label.color[2] = Color.b;
        label.color[3] = Color.a;

        cmdBuffer->GetHandle().insertDebugUtilsLabelEXT(label,VK::Instance::Get().GetDynamicLoader());
    }
};
struct BeginGroupCommand : Command
{
    std::string LabelName;
    vec4 Color;

    BeginGroupCommand(std::string LabelName, vec4 Color) : LabelName(std::move(LabelName)), Color(Color)
    {
    }

    std::string GetName() override
    {
        return "BeginGroup: " + LabelName;
    }

    void Execute(GPURef<VK::CommandBuffer> cmdBuffer) override
    {
        vk::DebugUtilsLabelEXT label;
        label.pLabelName = LabelName.c_str();
        label.color[0] = Color.r;
        label.color[1] = Color.g;
        label.color[2] = Color.b;
        label.color[3] = Color.a;
        cmdBuffer->GetHandle().beginDebugUtilsLabelEXT(label,VK::Instance::Get().GetDynamicLoader());
    }
};

struct EndGroupCommand : Command
{
    std::string GetName() override
    {
        return "EndGroup";
    }

    void Execute(GPURef<VK::CommandBuffer> cmdBuffer) override
    {
        cmdBuffer->GetHandle().endDebugUtilsLabelEXT(VK::Instance::Get().GetDynamicLoader());
    }
};

} // namespace VK
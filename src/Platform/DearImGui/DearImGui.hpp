#pragma once
#include <pch.h>
#include "misc/Singleton.hpp"
#include "Graphics/Vulkan/VKEnums.hpp"
#include "Render/Compositor/Pipeline.hpp"
class DearImGui : public Singleton<DearImGui>
{
    VK::Format CurrentFormat = VK::Format::eRGBA8_UNorm;
    std::shared_ptr<Render::Composite::Pipeline> Pipeline;
    public:
    DearImGui();
    void BeginFrame();
    void EndFrame();
    void UpdatePlatforms();
};
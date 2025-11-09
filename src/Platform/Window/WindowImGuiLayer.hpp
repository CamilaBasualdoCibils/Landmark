#pragma once
#include "pch.h"
#include "Render/Compositor/Layer.hpp"

namespace Platform
{
    class Window;
    class WindowImGuiLayer : public Render::Composite::Layer
{
    ImGui_ImplVulkanH_Window IMGUI_WINDOW;

    bool Build() override;
    void Compose(Render::Composite::Payload& payload) override;
    public:
    WindowImGuiLayer();
};
} // namespace Platform


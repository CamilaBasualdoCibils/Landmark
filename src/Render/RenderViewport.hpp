#pragma once
#include <Editor/EditorToolItem.hpp>
#include <Editor/EditorWindow.hpp>
#include <pch.h>
#include <Render/Types/Camera.hpp>
#include <Render/Types/Transform.hpp>
#include "Render/Components/Entity.hpp"
class RenderViewport : public Editor::ToolItem, Editor::Window<>
{
    std::optional<Entity> SelectedCamera;
    Entity ViewportCameraEntity;
    bool Open = true;
    void DrawWindowContents() override;
    void DrawTool() override
    {
        if (Open)
            Editor::Window<>::Draw();
    }
    void DrawHandle() override
    {
        ImGui::MenuItem(GetLabel().c_str(), nullptr, &Open);
    }
    public:
    RenderViewport();
};
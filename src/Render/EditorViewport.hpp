#pragma once
#include <Editor/EditorToolItem.hpp>
#include <Editor/EditorWindow.hpp>
#include <pch.h>
#include <Render/Camera/Camera.hpp>
#include <Transform/Transform.hpp>
#include "Entity/Entity.hpp"
class EditorViewport : public Editor::ToolItem, Editor::Window<>
{
    Camera editorCamera;
    Transform editorTransform;
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
    EditorViewport();
};
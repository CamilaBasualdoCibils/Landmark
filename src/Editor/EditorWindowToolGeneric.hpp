#pragma once
#include <pch.h>
#include "EditorToolItem.hpp"
#include "EditorWindow.hpp"
namespace Editor
{
    class WindowToolGeneric: private ToolItem,private Window<>
    {
        std::string Name;
        bool Open = false;
        void DrawWindowContents() override
        {
            WindowContents();
        }
        void DrawHandle() override
        {
            if (ImGui::Selectable(Name.c_str(),&Open))
            {
                Window<>::SetOpen(Open);
            }
        }
        void DrawTool() override
        {
            if (Open)
            {
                Window<>::Draw();
            }
        }
        public:
        WindowToolGeneric(const std::string& Name) : ToolItem(Name),Window<>(Name),Name(Name) {}
        void SetOpen(bool s) {Open = s;}
        bool IsOpen() const {return Open;}
        virtual void WindowContents() = 0;

    };
}
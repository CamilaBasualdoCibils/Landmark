#pragma once
#include <pch.h>
#include <Editor/EditorTool.h>
#include <Editor/EditorWindow.hpp>
class Console  : EditorTool {

    public:
    Console();
     void DrawWindowContents() override;
};
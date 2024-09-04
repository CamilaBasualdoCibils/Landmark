#include "EditorConsole.h"

void EditorConsole::Draw()
{
   //ImGui::InputTextMultiline()
}

void EditorConsole::DrawHandle()
{
    ImGui::MenuItem("Console", 0, &Open);
}

EditorConsole::EditorConsole() : EditorTool("Console")
{
}
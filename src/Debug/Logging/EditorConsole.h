#include <pch.h>
#include <Debug/EditorTool.h>
class EditorConsole : EditorTool {
    bool Open = true;
    void Draw() override;
    void DrawHandle() override;
    public:
    EditorConsole();
};
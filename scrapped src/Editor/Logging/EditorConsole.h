#include <pch.h>
#include <Editor/EditorTool.h>
class EditorConsole : EditorTool {
    bool Open = true;
    void Draw() override;
    void DrawHandle() override;
    public:
    EditorConsole();
};
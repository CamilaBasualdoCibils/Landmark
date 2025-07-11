#pragma once
#include "EditorObject.hpp"
#include "EditorWindow.hpp"
class Editor;

class EditorTool : public EditorObject, public EditorWindow<>
{

  private:
    /* data */
  protected:
  public:
    EditorTool(const std::string &name);
    void DrawTool() override;
    // draws the handle(s) to enable or disable this tool at the top of the editor
    void DrawHandle() override;
    void SetIsOpen(bool o)
    {
        EditorWindow::SetOpen(o);
    }
    constexpr bool GetIsOpen() const
    {
        return EditorWindow::IsOpen();
    }
};

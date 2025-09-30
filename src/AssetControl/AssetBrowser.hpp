#include "Editor/EditorToolItem.hpp"
#include "Editor/EditorWindowToolGeneric.hpp"
#include "pch.h"
class AssetBrowser : public Editor::WindowToolGeneric
{
    void WindowContents() override;

  public:
    AssetBrowser();
};
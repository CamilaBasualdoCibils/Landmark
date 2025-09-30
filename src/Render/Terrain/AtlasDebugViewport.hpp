#pragma once
#include "Types/SpatialTrees/Octree.hpp"
#include "misc/Singleton.hpp"
#include "misc/string_utils.h"
#include <Editor/EditorWindowToolGeneric.hpp>
#include <pch.h>

class AtlasDebugViewport : public Editor::WindowToolGeneric
{
  struct a {
    float v,b,c;
  };
    Octree<11, a> octree;


  public:
    AtlasDebugViewport();
    void WindowContents() override;
};
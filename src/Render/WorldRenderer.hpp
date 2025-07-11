
#pragma once
#include <pch.h>

#include "WorldCompositor.hpp"
#include <Graphics/Compositor/Compositor.hpp>
#include <misc/Singleton.hpp>
#include <Editor/Editor.hpp>
#include "RenderViewport.hpp"
class WorldRenderer : public Singleton<WorldRenderer>
{

    std::shared_ptr<Graphics::CompositeGroup> WorldCompositeGroup = Graphics::Compositor::Get().MakeRootGroup(
        Graphics::CompositeLayerProperties{.Name = "World Renderer Composite Group",.ResolutionOverride = uvec2{1000,1000}});
    std::shared_ptr<WorldCompositor> Compositor = WorldCompositeGroup->AddLayer<WorldCompositor>();

    public:
    WorldRenderer() {
        Editor::Editor::Get().GetMainToolGroup().GetOrPushToolBar("View")->PushObject<RenderViewport>();
    }
    decltype(WorldCompositeGroup) GetCompositeGroup() const {return WorldCompositeGroup;}
};
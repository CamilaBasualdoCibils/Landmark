#include "World.hpp"
#include "Editor/Editor.hpp"
World::World()
{
    worldOutliner = Editor::Editor::Get().GetMainToolGroup().GetOrPushToolBar("View")->PushObject<WorldOutliner>();
    entityInspector = Editor::Editor::Get().GetMainToolGroup().GetOrPushToolBar("View")->PushObject<EntityInspector>();
    renderViewport = Editor::Editor::Get().GetMainToolGroup().GetOrPushToolBar("View")->PushObject<RenderViewport>();
}

void World::Update()
{
    static double lastTime = glfwGetTime(); // Initialize at start

    // Inside your update loop:
    double currentTime = glfwGetTime();
    DeltaTime = currentTime - lastTime;
    lastTime = currentTime;
}

Entity World::CreateEntity()
{

    Entity e(registry.create());
    return e;
}

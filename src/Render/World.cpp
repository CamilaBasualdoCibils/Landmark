#include "World.hpp"
#include "Editor/Editor.hpp"
World::World()
{
   worldOutliner = Editor::Editor::Get().GetMainToolGroup().GetOrPushToolBar("View")->PushObject<WorldOutliner>();
   entityInspector = Editor::Editor::Get().GetMainToolGroup().GetOrPushToolBar("View")->PushObject<EntityInspector>();
}

void World::Update()
{

}
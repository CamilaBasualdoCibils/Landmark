#include "SceneManager.hpp"
#include "Scene.hpp"
#include <ECS/EditorTools/EditorEntityList.h>


SceneManager::SceneManager()
{
    editor_inspector = Editor::Get().GetMainToolGroup().GetOrPushGroup("Windows")->PushObject<EditorEntityList>(0);
}

SceneManager::ScenePointer SceneManager::CreateScene(const std::string &name)
{
    SceneID id = next_id++;
    auto ss = std::make_shared<Scene>(id,name);
    const auto loc = scenes.insert(std::make_pair(id,ss));
   //const_cast<SceneID&>((*ss).ID)= id;
    return ss;
}
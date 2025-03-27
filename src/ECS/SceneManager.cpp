#include "SceneManager.hpp"
#include "Scene.hpp"
#include <ECS/EditorTools/EditorEntityList.h>
void SceneManager::Init()
{
    editor_inspector = &Editor::GetInstance()->GetMainToolGroup().GetOrPushGroup("Windows").PushObject<EditorEntityList>(0);
}


std::vector<AppModule::EngineCallInject> SceneManager::GetInjections()
{
    EngineCallInject init_call("SceneManager Init");
    init_call.call = std::bind(&SceneManager::Init,this);
    init_call.call_point = EngineCallPoints::START;
    init_call.priority = 0;

    std::vector<EngineCallInject> injects = {init_call};
    return injects;
}
SceneManager::ScenePointer SceneManager::CreateScene(const std::string& name)
{
    SceneID id = next_id++;
    auto ss = std::make_shared<Scene>(id,name);
    const auto loc = scenes.insert(std::make_pair(id,ss));
   //const_cast<SceneID&>((*ss).ID)= id;
    return ss;
}
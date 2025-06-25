#include "SceneObject.hpp"
#include <Core/App.h>
#include <ECS/SceneManager.hpp>
#include <ECS/Scene.hpp> 
std::shared_ptr<Scene> SceneObject::GetScene()
{
    return App::GetInstance()->GetModule<SceneManager>()->GetScene(scene_id);
}
/*
IComponentData *SceneObject::GetComponent(ComponentTypeID type)
{
    return GetScene()->GetComponent(ID,type);
}
*/
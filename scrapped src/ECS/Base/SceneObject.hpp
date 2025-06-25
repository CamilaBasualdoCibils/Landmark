#pragma once
#include "Object.hpp"
#include <ECS/Handles/Component.hpp>
class Scene;
struct SceneObject : Object
{

    SceneID scene_id;

    SceneObject(SceneID scene, ObjectID id) : Object(id), scene_id(scene)
    {
    }
    std::shared_ptr<Scene> GetScene() ;
   // IComponentData* GetComponent(ComponentTypeID type) const;
    template <typename T>
    Component<T> GetComponent() const {
        return Component<T>(scene_id,ID);
    }
    

};
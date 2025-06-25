#pragma once
#include "ObjectHandle.hpp"

//using Entity = ObjectPointer<EntityData>;
#include <ECS/Handles/Component.hpp>
class EntityData;
class Entity : public SceneObjectHandle<EntityData> {
    public:
    EntityData* operator->() override;
    EntityData& operator*() override;
    Entity():SceneObjectHandle() {}
    Entity(SceneID _scene_id, ObjectID _object_id):SceneObjectHandle(_scene_id,_object_id){};

    template <typename T>
    Component<T> AddComponent();
};

template <typename T>
inline Component<T> Entity::AddComponent()
{
    GetScene()->AddComponent(GetObjectID(),ComponentRegistry::GetComponentID<T>());
    return Component<T>(GetSceneID(),GetObjectID());
}

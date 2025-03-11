#pragma once
#include <pch.h>

#include <ECS/Defines.hpp>
#include "ObjectHandle.hpp"

#include <ECS/Scene.hpp>
#include <ECS/ComponentRegistry.hpp>
class IComponentData;
template <typename T> //, typename std::enable_if_t<std::is_base_of_v<IComponentData, T>>>
class Component : public SceneObjectHandle<T>
{
public:
    Component(SceneID _scene_id, ObjectID _object_id) : SceneObjectHandle<T>(_scene_id, _object_id)
    {
    }
    Component() = default;
    T *operator->() override;

    T &operator*() override;
};

template <typename T>
inline T *Component<T>::operator->()
{
    IComponentData *comp = this->GetScene()->GetComponent(this->GetObjectID(), ComponentRegistry::GetComponentID<T>());
    T *c_comp = reinterpret_cast<T *>(comp);
    LASSERT(c_comp, "Invalid Component Type");
    return c_comp;
}

template <typename T>
inline T &Component<T>::operator*()
{
    return *operator->();
}

#pragma once
#include "Entity.hpp"
#include "EntityCompDefines.hpp"
#include <pch.h>
class EntityInfo;
class TransformComponent;
class Entity
{
    EntityID ID;

  public:
    Entity(EntityID ID);
    template <typename T> T &GetComponent()
    {
        return GetRegistry().get<T>(ID);
    }
    template <typename T> const T &GetComponent() const
    {
        return GetRegistry().get<T>(ID);
    }
    template <typename T> T &AddComponent()
    {
        return GetRegistry().emplace<T>(ID, ID);
    }
    template <typename T> bool HasComponent() const
    {
        return GetRegistry().all_of<T>(ID);
    };
    template <typename T> T &AddOrGetComponent()
    {
        return HasComponent<T>() ? GetComponent<T>() : AddComponent<T>();
    }
    EntityInfo& GetInfo() ;
    const EntityInfo& GetInfo() const;
    TransformComponent& GetTransform() ;
    const TransformComponent& GetTransform() const;
    EntityID GetID() const {return ID;}
    private:
    static entt::registry& GetRegistry();
};
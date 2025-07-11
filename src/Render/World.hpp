#pragma once
#include "EntityInspector.hpp"
#include "WorldOutliner.hpp"
#include <misc/Singleton.hpp>
#include <pch.h>
#include "Components.hpp"
class World : public Singleton<World>
{

    std::shared_ptr<WorldOutliner> worldOutliner;
    std::shared_ptr<EntityInspector> entityInspector;
    entt::registry registry;

  public:
    World();

    void Update();
    auto CreateEntity()
    {
        auto entity = registry.create();
        registry.emplace<EntityInfo>(entity);
        registry.emplace<Transform>(entity);
        return entity;
    }
    template <typename T>
    auto& GetComponent(entt::entity entity)
    {
        return registry.get<T>(entity);
    }
    template <typename T>
    auto& AddComponent(entt::entity entity)
    {
        return registry.emplace<T>(entity);
    }
    template <typename... TYPES>
    auto GetComponentView()
    {
        return registry.view<TYPES...>();
    }
    decltype(worldOutliner) GetWorldOutliner() const
    {
        return worldOutliner;
    }
};
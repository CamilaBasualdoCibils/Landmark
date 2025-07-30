#pragma once
#include "Components/EntityInfo.hpp"
#include "Components/Entity.hpp"
#include "EntityInspector.hpp"
#include "Render/Types/Transform.hpp"
#include "RenderViewport.hpp"
#include "WorldOutliner.hpp"
#include <misc/Singleton.hpp>
#include <pch.h>
class World : public Singleton<World>
{

    std::shared_ptr<WorldOutliner> worldOutliner;
    std::shared_ptr<EntityInspector> entityInspector;
    std::shared_ptr<RenderViewport> renderViewport;
    entt::registry registry;
    double DeltaTime = 0.0f;

  public:
    World();

    void Update();
    Entity CreateEntity();
    entt::registry &GetRegistry()
    {
        return registry;
    }

    template <typename... TYPES> auto GetComponentView()
    {
        return registry.view<TYPES...>();
    }
    decltype(worldOutliner) GetWorldOutliner() const
    {
        return worldOutliner;
    }
    double GetDeltaTime() const
    {

        return DeltaTime;
    }
};
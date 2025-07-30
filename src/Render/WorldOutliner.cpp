#include "WorldOutliner.hpp"
#include "World.hpp"
void WorldOutliner::DrawWindowContents()
{
    auto EntityInfoView = World::Get().GetComponentView<const EntityInfo>();
    for (entt::entity entity : EntityInfoView)
    {
        const auto& entInfo = EntityInfoView.get<const EntityInfo>(entity);
         bool selected = SelectedEntity.has_value() && entity  == SelectedEntity->GetID();
        ImGui::PushID((uint32_t)entity);
          if (ImGui::Selectable(entInfo.Name.c_str(), &selected))
          if (selected) SelectedEntity = Entity(entity);

        ImGui::PopID();
    }
    //for (const auto &entityEntry : World::Get().GetEntities())
    //{
    //    std::shared_ptr<Entity> entity = entityEntry.second;
    //    bool selected = SelectedEntity.has_value() && entity->EntityID == *SelectedEntity;
    //    if (ImGui::Selectable(entity->Name.c_str(), &selected))
    //    {
    //        if (selected) SelectedEntity = entity->EntityID;
    //    }
    //}
}

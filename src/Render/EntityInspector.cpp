#include "EntityInspector.hpp"
#include "World.hpp"
void EntityInspector::DrawWindowContents()
{
    std::optional<entt::entity> entity = World::Get().GetWorldOutliner()->GetSelectedEntity();

    if (!entity.has_value())
        return;

    // std::shared_ptr<Entity> entity = World::Get().GetEntity(*entityID);
    EntityInfo &entInfo = World::Get().GetComponent<EntityInfo>(*entity);
    // if (!entity) return;

    ImGui::Checkbox("##Enabled", &entInfo.Enabled);

    ImGui::SameLine();
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputText("##name", &entInfo.Name);

    Transform &transform = World::Get().GetComponent<Transform>(*entity);

    transform.DrawInspector();
}

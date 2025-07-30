#include "EntityInspector.hpp"
#include "Components/TransformComponent.hpp"
#include "World.hpp"
void EntityInspector::DrawWindowContents()
{
    std::optional<Entity> entity = World::Get().GetWorldOutliner()->GetSelectedEntity();

    if (!entity.has_value())
        return;


    EntityInfo &entInfo = entity->GetInfo();
    const bool Locked = entInfo.Locked;
    if (Locked)
        ImGui::BeginDisabled();

    ImGui::Checkbox("##Enabled", &entInfo.Enabled);

    ImGui::SameLine();
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputText("##name", &entInfo.Name);

    ImGui::Separator();
    TransformComponent &transform = entity->GetTransform();

    transform.DrawInspector();

    if (Locked)
        ImGui::EndDisabled();
}

#include "AssetEnums.hpp"

bool AssetAvailabilityInspector(Flags<AssetAvailability> &av)
{
    ImGui::BeginChild("availability",ImVec2(0,0),ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_AutoResizeY);
    ImGui::Text("Availability:");
    ImGui::SameLine();
    bool cpu = av & AssetAvailability::eCPU,gpu = av & AssetAvailability::eGPU;
    bool modified = false;
    if (ImGui::Checkbox("CPU",&cpu)) {
        av.Set(AssetAvailability::eCPU,cpu);
        modified = true;
    }
    ImGui::SameLine();
    if (ImGui::Checkbox("GPU",&gpu)) {
        av.Set(AssetAvailability::eGPU,gpu);
        modified = true;
    }
    ImGui::EndChild();
    return modified;

}
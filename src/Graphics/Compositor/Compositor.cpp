#include "Compositor.hpp"
#include "Graphics/GraphicsEngine.hpp"
#include <misc/Conversions.hpp>
Graphics::Compositor::Compositor()
{
}

std::shared_ptr<Graphics::CompositeGroup> Graphics::Compositor::MakeRootGroup(
    const CompositeGroupProperties &GroupProperties, int32_t Priority)
{
    using EntryType = std::pair<size_t, std::shared_ptr<Graphics::CompositeGroup>>;
    EntryType NewEntry = {Priority, std::make_shared<Graphics::CompositeGroup>(GroupProperties)};

    auto it = std::lower_bound(RootGroups.begin(), RootGroups.end(), NewEntry,
                               [](const EntryType &A, const EntryType &B) { return A.first < B.first; });
    RootGroups.insert(it, NewEntry);
    return NewEntry.second;
}

void Graphics::Compositor::DrawDebug()
{
    static std::shared_ptr<Graphics::ICompositeLayer> SelectedLayer = nullptr;
    ImGui::Begin("Compositor");
    const ImVec2 AvailSpace = ImGui::GetContentRegionAvail();
    if (ImGui::BeginChild("Selected", {AvailSpace.x * 0.25f, AvailSpace.y},
                          ImGuiChildFlags_ResizeX | ImGuiChildFlags_Borders))
    {
        std::function<void(std::shared_ptr<Graphics::CompositeGroup>)> TreeRecursive =
            [&](std::shared_ptr<Graphics::CompositeGroup> Group) {
                int ID = 0;
                bool selected = Group == SelectedLayer;
                if (ImGui::TreeNodeEx(Group->GetProperties().Name.c_str(), (selected * ImGuiTreeNodeFlags_Selected) |
                                                                               ImGuiTreeNodeFlags_OpenOnArrow |
                                                                               ImGuiTreeNodeFlags_OpenOnDoubleClick))
                {
                    if (ImGui::IsItemClicked())
                        SelectedLayer = Group;
                    ImGui::PushID(ID);
                    for (const auto &Child : Group->GetLayers())
                    {
                        if (std::shared_ptr<Graphics::CompositeGroup> ChildGroup =
                                std::dynamic_pointer_cast<CompositeGroup>(Child))
                        {
                            TreeRecursive(ChildGroup);
                        }
                        else
                        {
                            bool selected = Child == SelectedLayer;
                            if (ImGui::Selectable(Child->GetProperties().Name.c_str(), &selected))
                            {
                                SelectedLayer = Child;
                            }
                        }
                    }
                    ImGui::PopID();
                    ID++;

                    ImGui::TreePop();
                }
            };
        int ID = 0;
        for (const auto &Group : RootGroups)
        {
            ImGui::PushID(ID);
            TreeRecursive(Group.second);
            ImGui::PopID();
            ID++;
        }
    }
    ImGui::EndChild();
    ImGui::SameLine();
    if (ImGui::BeginChild("Inspector", {0, 0}, ImGuiChildFlags_Borders))
    {

        if (SelectedLayer)
        {
            auto LayerResolution = SelectedLayer->GetResolution();
            float LayerRatio = (float)LayerResolution.x / (float)LayerResolution.y;
            if (ImGui::BeginTabBar("Attachments"))
            {
                for (const auto &Attachment : SelectedLayer->GetAttachments())
                {

                    if (ImGui::BeginTabItem(Attachment.first.c_str(), nullptr, ImGuiTabItemFlags_NoReorder))
                    {

                        const auto AvailableRegion = ImGuiToGlm(ImGui::GetContentRegionAvail());
                        const float AvailableRegionRatio = (float)AvailableRegion.x / (float)AvailableRegion.y;

                        uvec2 ImageSize;
                        if (AvailableRegionRatio > LayerRatio)
                        {
                            ImageSize.y = AvailableRegion.y;
                            ImageSize.x = ImageSize.y * LayerRatio;
                        }
                        else
                        {
                            ImageSize.x = AvailableRegion.x;
                            ImageSize.y = ImageSize.x / LayerRatio;
                        }
                        ImGui::Image((ImTextureID)Attachment.second->GetImguiDescriptorSet(),
                                     GlmToImGui(ImageSize));
                        ImGui::EndTabItem();
                    }
                }

                ImGui::EndTabBar();
            }
        }
    }
    ImGui::EndChild();
    // ImGui::Image((ImTextureID)Group.second->GetOutput()->VK().GetImguiDescriptorSet(),
    // ImGui::GetContentRegionAvail());

    ImGui::End();
}

void Graphics::Compositor::RenderCompositions()
{
    for (const auto Group : RootGroups)
    {
        Graphics::CompositeContext context{};
        Graphics::CompositeLayerExecute RenderExecution = Group.second->OnRender(context);
        GraphicsEngine::Get().Push(RenderExecution.CommandManagers);
    }
}

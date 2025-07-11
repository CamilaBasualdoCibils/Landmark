#include "Compositor.hpp"
#include "Graphics/GraphicsEngine.hpp"
Graphics::Compositor::Compositor()
{
}

std::shared_ptr<Graphics::CompositeGroup> Graphics::Compositor::GetRootGroup(const std::string &Name)
{
    LASSERT(RootGroups.contains(Name), "Invalid Group");
    return RootGroups[Name];
}

bool Graphics::Compositor::ExistsRootGroup(const std::string &Name)
{
    return RootGroups.contains(Name);
}

std::shared_ptr<Graphics::CompositeGroup> Graphics::Compositor::MakeRootGroup(
    const CompositeLayerProperties &GroupProperties)
{
    LASSERT(!RootGroups.contains(GroupProperties.Name), "Invalid Group");

    std::shared_ptr<Graphics::CompositeGroup> Group = std::make_shared<Graphics::CompositeGroup>(GroupProperties);
    RootGroups.emplace(std::make_pair(GroupProperties.Name, Group));
    return Group;
}

void Graphics::Compositor::DrawDebug()
{
    ImGui::Begin("Compositor");
    std::function<void(std::shared_ptr<Graphics::CompositeGroup>)> TreeRecursive =
        [&](std::shared_ptr<Graphics::CompositeGroup> Group) {
            if (ImGui::TreeNode(Group->GetProperties().Name.c_str()))
            {
                for (const auto &Child : Group->GetLayers())
                    if (std::shared_ptr<Graphics::CompositeGroup> ChildGroup =
                            std::dynamic_pointer_cast<CompositeGroup>(Child))
                    {
                        TreeRecursive(ChildGroup);
                    }
                    else
                    {
                        ImGui::TextUnformatted(Child->GetProperties().Name.c_str());
                    }
                ImGui::TreePop();
            }
        };
    for (const auto &Group : RootGroups)
    {
        TreeRecursive(Group.second);
    }

    // ImGui::Image((ImTextureID)Group.second->GetOutput()->VK().GetImguiDescriptorSet(),
    // ImGui::GetContentRegionAvail());

    ImGui::End();
}

void Graphics::Compositor::RenderCompositions()
{
    for (const auto &Group : RootGroups)
    {
        Graphics::CompositeContext context{};
        Graphics::CompositeLayerExecute RenderExecution = Group.second->OnRender(context);
        GraphicsEngine::Get().Push(RenderExecution.CommandManagers);
    }
}

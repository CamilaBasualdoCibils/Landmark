#include "Compositor.hpp"

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

    for (const auto &Group : RootGroups)
    {
        if (ImGui::TreeNode(Group.first.c_str()))
        {
            //ImGui::Image(Group.second->GetOutput()->)

            ImGui::TreePop();
        }
    }

    ImGui::End();
}

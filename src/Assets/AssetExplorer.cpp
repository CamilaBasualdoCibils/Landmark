#include "AssetExplorer.hpp"
#include "Inspectors/Texture/TextureAssetInspector.hpp"
#include <Editor/EditorIcons.hpp>
static void TreeViewRecursive(const Folder &folder, Folder &selected_folder, bool open_first_time)
{
    const bool selected = selected_folder == folder;
    int open_flag = ImGuiTreeNodeFlags_Selected * selected;
    ImGui::SetNextItemOpen(open_first_time, ImGuiCond_Once);
    bool open = ImGui::TreeNodeEx(folder.GetName().c_str(), open_flag | ImGuiTreeNodeFlags_OpenOnArrow);

    if (ImGui::IsItemClicked())
        selected_folder = folder.GetPath();
    if (open)
    {
        for (const Folder &f : folder.GetSubFolders())
        {
            TreeViewRecursive(f, selected_folder, false);
        }
        ImGui::TreePop();
    }

    // ImGui::TreeNodeEx(folder.GetName(),0)
}
void AssetExplorer::DrawWindowContents()
{

    ImGui::BeginChild("options", ImVec2(0, 0), ImGuiChildFlags_AutoResizeY);

    ImGui::SameLine();
    ImGui::SliderInt("##grid size", &grid_size, 25, 500, "size: %d");
    ImGui::EndChild();

    ImGui::BeginChild("Tree View", ImVec2(300, 0), ImGuiChildFlags_ResizeX | ImGuiChildFlags_Borders | ImGuiChildFlags_NavFlattened);

    TreeViewRecursive(IO::GetAssets(), current_folder, true);
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::BeginChild("directory and path control");
    ImGui::BeginChild("##path control", ImVec2(0, 0), ImGuiChildFlags_AutoResizeY);
    if (ImGui::ArrowButton("go up", ImGuiDir_Up))
    {
        current_folder = current_folder.GetParentFolder();
    }
    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
    std::string current_path = current_folder.GetPath();
    if (ImGui::InputText("##path", &current_path))
    {
        Folder f(current_path);
        if (f.Exists())
            current_folder = f;
    }
    ImGui::PopItemWidth();
    ImGui::EndChild();

    ImGui::BeginChild("directory explorer", ImVec2(0, 0));
    DrawFolderContents();
    ImGui::EndChild();
    ImGui::EndChild();
}

void AssetExplorer::DrawTool()
{
    EditorTool::DrawTool();
    for (auto it = inspectors.begin(); it != inspectors.end(); it++)
    {
        AssetInspector &inspector = **it;
        if (!inspector.IsOpen())
        {
            inspectors.erase(it);
            it--;
            continue;
        }
        inspector.Draw();
    }
}

void AssetExplorer::DrawFolderContents()
{
    const ImVec2 file_display_available_space = ImGui::GetContentRegionAvail();
    const auto& io = ImGui::GetStyle();
    int grid_count_x = file_display_available_space.x / (grid_size+io.ItemSpacing.x);
    int count_x = 0;
    for (const auto f : current_folder.GetSubFolders())
    {
        const std::string &folder_name = f.GetName();
        DrawFileSystemObjectIcon(f, grid_size);

        bool clicked = ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0);
        if (clicked)
        {
            current_folder = f;
        }
        count_x++;
        if (count_x == grid_count_x)
        {
            count_x = 0;
        }
        else
            ImGui::SameLine();
    }

    for (const auto f : current_folder.GetFiles())
    {
        const std::string &file_name = f.GetName();
        if (f.PathEndsWith(Asset::descriptor_file_extension))
            continue;
        DrawFileSystemObjectIcon(f, grid_size);
        bool clicked = ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0);
        if (clicked)
        {
            Asset asset(f.GetPath());
            openAssetInspector(asset);
        }
        count_x++;
        if (count_x == grid_count_x)
        {
            count_x = 0;
        }
        else
            ImGui::SameLine();
    }
}

std::shared_ptr<AssetInspector> AssetExplorer::openAssetInspector(Asset &asset)
{
    switch (asset.GetType())
    {
    case AssetType::eTEXTURE:
        /* code */
        inspectors.emplace_back(std::make_shared<TextureAssetInspector>());
        inspectors.back()->SetAsset(asset);
        inspectors.back()->SetOpen(true);
        break;

    default:
        return nullptr;
        break;
    }
    return inspectors.back();
}

void AssetExplorer::DrawFileSystemObjectIcon(const FileSystemObject &f, int space)
{
    EditorIcons::Icon icon;
    if (f.isFolder())
    {
        Folder folder(f.GetPath());
        icon = EditorIcons::GetIcon("EditorUI.png", folder.isEmpty() ? "Folder" : "Folder Filled");
    }
    else
    {
        Asset asset(f.GetPath());

        switch (asset.GetType())
        {
        case AssetType::eTEXTURE:
            icon = EditorIcons::GetIcon("EditorUI.png", "Picture");
            break;
        case AssetType::eASEPRITE:
            icon = EditorIcons::GetIcon("EditorUI.png", "Aseprite");

            break;
        default:
            icon = EditorIcons::GetIcon("EditorUI.png", "Document Question");
            break;
        }
    }
    const auto& io = ImGui::GetIO();
    const vec2 icon_size = icon.first.uv_max - icon.first.uv_min;
    const bool x_greater = icon_size.x > icon_size.y;
    const float icon_aspect = icon_size.x / icon_size.y;
    if (ImGui::BeginChild(f.GetName().c_str(), ImVec2(space, space)))
    {
        std::string label = f.GetName();
        const ImVec2 available_space = ImGui::GetContentRegionAvail();
        /*
        int Num_characters_fit = ImGui::GetContentRegionMax().x/ImGui::GetFontSize();
        if (label.length() > Num_characters_fit) {
            label = label.substr(0,std::floor(Num_characters_fit)-3);
            label += "...";
        }*/
        const ImVec2 label_size = ImGui::CalcTextSize(label.c_str());
        int icon_v_space = available_space.y - label_size.y - ImGui::GetStyle().ItemSpacing.y;
        ImVec2 image_size = {icon_v_space, icon_v_space};
        if (x_greater)
            image_size.y /= icon_aspect;
        else
            image_size.x *= icon_aspect;

        const ImVec2 min = {icon.first.uv_min.x, icon.first.uv_min.y}, max = {icon.first.uv_max.x, icon.first.uv_max.y};
        // ImGui::SetCursorPos(ImGui::GetCursorPos() + )
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2{-image_size.x / 2 + available_space.x / 2, 0});
        ImGui::Image(icon.second, image_size, min, max);


        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2{-label_size.x / 2 + available_space.x / 2, 0});
        ImGui::Text(label.c_str());
    }
    ImGui::EndChild();
}

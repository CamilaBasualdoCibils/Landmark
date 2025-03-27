#include "AssetExplorer.hpp"
#include "Inspectors/TextureAssetInspector.hpp"
static void TreeViewRecursive(const Folder &folder, Folder& selected_folder,bool open_first_time)
{
    const bool selected = selected_folder == folder;
    int open_flag =  ImGuiTreeNodeFlags_Selected * selected;
    ImGui::SetNextItemOpen(open_first_time, ImGuiCond_Once);
    bool open = ImGui::TreeNodeEx(folder.GetName().c_str(),open_flag | ImGuiTreeNodeFlags_OpenOnArrow);

    if (ImGui::IsItemClicked())
        selected_folder = folder.GetPath();
    if (open)
    {
        for (const Folder &f : folder.GetSubFolders())
        {
            TreeViewRecursive(f, selected_folder,false);
        }
        ImGui::TreePop();
    }

    // ImGui::TreeNodeEx(folder.GetName(),0)
}
void AssetExplorer::DrawWindowContents()
{

    ImGui::BeginChild("options",ImVec2(0,0),ImGuiChildFlags_AutoResizeY);

   
    ImGui::SameLine();
    ImGui::SliderInt("##grid size",&grid_size,25,500,"size: %d");
    ImGui::EndChild();

    ImGui::BeginChild("Tree View", ImVec2(300, 0), ImGuiChildFlags_ResizeX | ImGuiChildFlags_Borders | ImGuiChildFlags_NavFlattened);
    
    TreeViewRecursive(IO::GetAssets(), current_folder,true);
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::BeginChild("directory and path control");
    ImGui::BeginChild("##path control",ImVec2(0,0),ImGuiChildFlags_AutoResizeY);
    if (ImGui::ArrowButton("go up",ImGuiDir_Up)) {
        current_folder = current_folder.GetParentFolder();
    }
    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
    std::string current_path = current_folder.GetPath();
    if (ImGui::InputText("##path",&current_path)) {
        Folder f(current_path);
        if (f.Exists()) current_folder = f;
    }
    ImGui::PopItemWidth();
    ImGui::EndChild();


    ImGui::BeginChild("directory explorer",ImVec2(0,0));
    DrawFolderContents();
    ImGui::EndChild();
    ImGui::EndChild();

}

void AssetExplorer::DrawTool()
{
    EditorTool::DrawTool();
    for( auto it = inspectors.begin(); it != inspectors.end();it++) {
        AssetInspector& inspector = **it;
        if (!inspector.isOpen()) {
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
    int grid_count_x  = file_display_available_space.x /grid_size;
    int count_x = 0;
    for (const auto f : current_folder.GetSubFolders()) {
        const std::string& folder_name = f.GetName();
        ImGui::BeginChild(folder_name.c_str(),ImVec2(grid_size,grid_size),ImGuiChildFlags_Borders);
        ImGui::TextColored(ImVec4(0.3,0.8,0.0,1.0),folder_name.c_str());

        ImGui::EndChild();
        bool clicked = ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0);
        if (clicked) {
            current_folder = f;
        }
        count_x ++;
        if (count_x == grid_count_x) {
            count_x = 0;
        }
        else ImGui::SameLine();
    }

    for (const auto f : current_folder.GetFiles()) {
        const std::string& file_name = f.GetName();
        if (f.PathEndsWith(Asset::descriptor_file_extension)) continue;
        ImGui::BeginChild(file_name.c_str(),ImVec2(grid_size,grid_size),ImGuiChildFlags_Borders);
        ImGui::TextColored(ImVec4(0.7,0.5,0.0,1.0),file_name.c_str());
        ImGui::EndChild();
        bool clicked = ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0);
        if (clicked) {
            Asset asset(f.GetPath());
            openAssetInspector(asset);
        }
        count_x ++;
        if (count_x == grid_count_x) {
            count_x = 0;
        }
        else ImGui::SameLine();
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
        break;
    
    default:
    return nullptr;
        break;
    }
    return inspectors.back();
}

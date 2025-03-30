#pragma once
#include <pch.h>
#include <Editor/EditorTool.h>
#include <IO/Folder.h>
#include <IO/File.h>
#include <IO/IO.h>
#include "Inspectors/AssetInspector.hpp"
class AssetExplorer : public EditorTool
{

    Folder current_folder = IO::GetAssets();
    int grid_size = 200;
    std::vector<std::shared_ptr<AssetInspector>> inspectors;

public:
    AssetExplorer() : EditorTool("Asset Explorer")
    {
    }

    void DrawWindowContents() override;
    void DrawTool() override;

private:
    void DrawFolderContents();
    std::shared_ptr<AssetInspector> openAssetInspector(Asset &asset);
    void DrawFileSystemObjectIcon(const FileSystemObject& f,int size);
};

#include "AssetRegistry.hpp"
#include "AssetBrowser.hpp"
#include "AssetGlobals.hpp"
#include "Editor/Editor.hpp"
#include "IO/File/File.hpp"
#include "AssetDescriptor.hpp"
void AssetRegistry::ParseFile(File file)
{
    std::cerr << file.GetPath() << std::endl;
    if (file.GetLastExtension() != LandmarkFileExtension)
    {
        File importedFile;
        Asset importedAsset;
        ImportAsset(file, importedFile, importedAsset);
    }
}



void AssetRegistry::ImportAsset(File file, File &outImported, Asset &outAsset)
{
    AssetUUID UUID = AssetUUIDGenerator();
    std::ostringstream oss;
    oss << UUID;
    std::string uuid_str = oss.str();

    LASSERT(file.Copy(ImportFolder, uuid_str, FileCopyOptions::eOverwrite),"Unable to copy file");
    AssetDescriptor descriptor;
    //descriptor.Name = file.GetName();
    
    std::cout << "Imported file: " << file.GetName() << "to: " << uuid_str << std::endl;
}

AssetRegistry::AssetRegistry()
{
    if (!ImportFolder.Exists()) ImportFolder.Create();
    Editor::Editor::Get()
        .GetMainToolGroup()
        .GetOrPushToolBar(Editor::Editor::Get().Tool_ToolBar)
        ->PushObject<AssetBrowser>();

}

AssetRegistry::~AssetRegistry()
{
}

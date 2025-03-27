#pragma once
#include <pch.h>
#include <Editor/EditorTool.h>
#include <Assets/Asset.hpp>
class AssetInspector {
    std::optional<Asset> asset;
    bool open = true;
    std::string name = "default inspector name";
    protected:

    virtual void DrawContents() = 0;
    public:
    virtual void Draw();
    virtual void SetAsset(const Asset& asset);
    virtual Asset GetAsset() const {return *asset;}
    void SetName(const std::string& _name) {name = _name;}
    std::string GetName() const {return name;}
    bool isOpen() {return open;}
};
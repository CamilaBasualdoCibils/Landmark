#pragma once
#include <pch.h>
//#include <Editor/EditorTool.h>
#include <Assets/Asset.hpp>
#include <Editor/EditorWindow.hpp>
class AssetInspector: public EditorWindow<> {
    std::optional<Asset> asset;



    public:
    virtual void SetAsset(const Asset& asset);
    virtual Asset GetAsset() const {return *asset;}
    virtual ~AssetInspector() {};

};
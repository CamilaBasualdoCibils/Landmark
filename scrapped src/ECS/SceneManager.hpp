#pragma once
#include <pch.h>
#include <misc/Singleton.hpp>
#include <Types/ObjectAllocator-new.h>
#include <ECS/Base/Object.hpp>
class EditorEntityList;
#include "Editor/Editor.h"

class Scene;
class SceneManager : public Singleton<SceneManager>
{

    using ScenePointer = std::shared_ptr<Scene>;
    SceneID next_id = 0;
    std::map<SceneID,ScenePointer> scenes;
    //ObjectAllocator_new<ScenePointer, SceneID> scenes;
    std::shared_ptr<EditorEntityList> editor_inspector;
    
public:
    SceneManager();
    ScenePointer CreateScene(const std::string &name);

    const decltype(scenes)& GetScenes() const {return scenes;}
    decltype(scenes)& GetScenes() {return scenes;}

    ScenePointer GetScene(SceneID id) {return scenes[id];}
};

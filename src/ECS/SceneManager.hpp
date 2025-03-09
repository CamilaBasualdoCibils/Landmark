#pragma once
#include <pch.h>
#include "Core/AppModule.h"
#include <Types/ObjectAllocator-new.h>
#include <ECS/Base/Object.hpp>
class EditorEntityList;
#include "Debug/Editor.h"

class Scene;
class SceneManager : public AppModule
{

    using ScenePointer = std::shared_ptr<Scene>;
    SceneID next_id = 0;
    std::map<SceneID,ScenePointer> scenes;
    //ObjectAllocator_new<ScenePointer, SceneID> scenes;
    EditorEntityList* editor_inspector;
    void Init();
    
public:
    std::vector<EngineCallInject> GetInjections() override;

    ScenePointer CreateScene(const std::string &name);

    const decltype(scenes)& GetScenes() const {return scenes;}
    decltype(scenes)& GetScenes() {return scenes;}

    ScenePointer GetScene(SceneID id) {return scenes[id];}
};

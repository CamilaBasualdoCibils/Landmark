#pragma once
#include <pch.h>
#include <ECS/Base/Object.hpp>
#include <ECS/Scene.hpp>
#include <ECS/SceneManager.hpp>
#include <Core/App.h>
class Scene;
template <typename T>
class ObjectHandle {

    public:
    virtual T* operator->() =0;
    ObjectHandle() =default;
};

template <typename T>
class SceneObjectHandle :public ObjectHandle<T>{
    private:
  
    protected:
    std::optional<SceneID> scene_id;
    std::optional<ObjectID> object_id;
    public:
    constexpr bool isValid() const {return scene_id.has_value() && object_id.has_value();}
    SceneID GetSceneID() const {return *scene_id;}
    ObjectID GetObjectID() const {return *object_id;}

    SceneObjectHandle(SceneID _scene_id, ObjectID _object_id) :ObjectHandle<T>(){
        scene_id = _scene_id;
        object_id = _object_id;
    }
    SceneObjectHandle() :ObjectHandle<T>(){
        
  
    }
    std::shared_ptr<Scene> GetScene();
};

template <typename T>
inline std::shared_ptr<Scene> SceneObjectHandle<T>::GetScene()
{
    return App::GetInstance()->GetModule<SceneManager>()->GetScene(*scene_id);
}

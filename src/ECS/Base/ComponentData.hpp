#pragma once
#include <pch.h>
#include <ECS/Defines.hpp>
#include "Object.hpp"
#include <ECS/ComponentInfo.hpp>
#include <ECS/ComponentRegistry.hpp>
class Component_Info;
struct IComponentData  : SceneObject{
    
    IComponentData(SceneID s_id,ObjectID id) : SceneObject(s_id,id) {}

    virtual void DrawInspector() =0;
    static Component_Info GetComponentTypeInfo() {
       throw "Any class that extends IComponentData must define this static function"; 
    }
    template <typename T>
    Component<T> GetComponent() {
        
    }

};
/*
template <typename T>
struct ComponentData : IComponentData {
    static const ComponentTypeID type_id;
    ComponentData(SceneID s_id,ObjectID id):IComponentData(s_id,id) {}
};
template <typename T>
const ComponentTypeID ComponentData<T>::type_id =ComponentRegistry::RegisterComponent<T>();*/

/*
// 0 reserved
static inline ComponentTypeID component_type_next_issued_id = 1;
template <typename T,string_litteral Name>
struct ComponentData:IComponentData {

    ComponentData(SceneID s_id,ObjectID id) : IComponentData(s_id,id) {

    }


    private:

        

};*/
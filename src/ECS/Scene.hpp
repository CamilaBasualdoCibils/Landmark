#pragma once
#include <pch.h>
#include <ECS/Base/Object.hpp>
//#include <ECS/Base/EntityData.hpp>
//#include <ECS/Handles/Component.hpp>
#include <Types/data_allocator.hpp>
class IComponentData;
class EntityData;
class Entity;
template<typename T> class Component;
class Scene : public Object
{
    //ObjectAllocator_new<Entity, ObjectID> entities;
    data_allocator entities;
    std::map<ComponentTypeID, std::shared_ptr<data_allocator>> components_all;
    //std::map<ComponentTypeID, std::shared_ptr<ObjectAllocator_new<IComponentData, ObjectID>>> components_all;
    using ObjectIDArg = ObjectID;

public:
    Scene(ObjectID id, const std::string &_name);
    Entity CreateEntity(const std::string &name);
    EntityData* GetEntityData(ObjectIDArg);
    bool ExistsEntity(ObjectIDArg id) const;

    IComponentData* GetComponent(ObjectID entity_id, ComponentTypeID type);
    IComponentData* AddComponent(ObjectID entity_id,ComponentTypeID type);
    bool HasComponent(ObjectID entity_id,ComponentTypeID type);

    const std::string& GetName() const {return name;}
    const decltype(entities)& GetEntities() const {return entities;}
     decltype(entities)& GetEntities()  {return entities;}
    SceneID GetID() const {return ID;}
private:
    //ENTITY_COMP_TEMP_FUNC_HEADER
    //std::shared_ptr<ObjectAllocator_new<T, ObjectID>> GetComponentTypeStorage();
    //std::shared_ptr<ObjectAllocator_new<IComponentData, ObjectID>>GetComponentTypeStorage(ComponentTypeID type);
    std::shared_ptr<data_allocator> GetCompDataOfType(ComponentTypeID type);
    std::string name;
    
};
/*
ENTITY_COMP_TEMP_FUNC_HEADER
inline bool Scene::HasComponent(ObjectIDArg id)
{
    return GetComponentTypeStorage<T>()->Contains(id);
}

ENTITY_COMP_TEMP_FUNC_HEADER CompPointer<T> Scene::AddComponent(ObjectIDArg id)
{
    LASSERT(ExistsEntity(id),"Invalid Entity");
    LASSERT(!HasComponent<T>(id),"Component already exists");

    GetEntity(id)->GetComponents().push_back(T::GetTypeID());
    return GetComponentTypeStorage<T>()->Insert_Emplace(id,GetID(),id);
}

 ENTITY_COMP_TEMP_FUNC_HEADER std::shared_ptr<ObjectAllocator_new<T, ObjectID>> Scene::GetComponentTypeStorage()
{
    if (!components_all.contains(T::GetTypeID()))
        components_all[T::GetTypeID()] = std::reinterpret_pointer_cast<ObjectAllocator_new<IComponentData,ObjectID>>(std::make_shared<ObjectAllocator_new<T, ObjectID>>());
    auto t = components_all[T::GetTypeID()];
    return std::reinterpret_pointer_cast<ObjectAllocator_new<T,ObjectID>>(t);
}*/
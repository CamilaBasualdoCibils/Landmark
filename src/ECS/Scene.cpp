#include "Scene.hpp"
#include <ECS/Base/EntityData.hpp>
#include <ECS/Handles/Entity.hpp>
#include <ECS/Handles/Component.hpp>
#include <ECS/ComponentRegistry.hpp>
/*
EntityPointer Scene::CreateEntity(const std::string &name)
{
    auto ptr = entities.Push_Emplace(ID,-1, name);
    const_cast<ObjectID&>((*ptr).ID) = ptr.GetIndex();
    (*ptr).SetEnabled(true);
    return ptr;
}

EntityPointer Scene::GetEntity(ObjectIDArg id)
{
    LASSERT(entities.Contains(id),"Entity does not exist!");
    return entities.find(id);
}

bool Scene::ExistsEntity(ObjectIDArg id) const
{
    return entities.Contains(id);
}

ENTITY_COMP_TEMP_FUNC_HEADER CompPointer<T> Scene::GetComponent(ObjectIDArg id)
{
    return GetComponentTypeStorage<T>()[id];
}

IComponentData* Scene::GetComponent(ObjectID entity_id, ComponentTypeID type)
{
     auto& Comp_Storage = *GetComponentTypeStorage(type);
     LASSERT(IComponentData::component_registry.contains(type),"Component Size Not Registered");
     const size_t size = IComponentData::component_registry[type].size;
    LASSERT(size >= sizeof(IComponentData),"Invalid Size");
    LASSERT(Comp_Storage.Contains(entity_id),"Invalid!");
    return (IComponentData*)((char*)Comp_Storage.data() + entity_id*size);



}


std::shared_ptr<ObjectAllocator_new<IComponentData, ObjectID>> Scene::GetComponentTypeStorage(ComponentTypeID type)
{
    return components_all[type];
}
*/

Scene::Scene(ObjectID id, const std::string &_name) : Object(id), name(_name), entities(sizeof(EntityData))
{
}

Entity Scene::CreateEntity(const std::string &name)
{
    const size_t index = entities.GetNextInsertLocation();
    EntityData edata(ID, index, name);

    entities.Insert_Emplace<EntityData>(index, edata);

    return Entity(ID, index);
}

EntityData *Scene::GetEntityData(ObjectIDArg ID)
{
    return reinterpret_cast<EntityData *>(entities[ID]);
}

IComponentData *Scene::GetComponent(ObjectID entity_id, ComponentTypeID type)
{
    LASSERT(HasComponent(entity_id, type), "Component Not Found!");
    const auto& comp_data =  GetCompDataOfType(type);
    return (IComponentData*)(*comp_data)[entity_id];
}

IComponentData *Scene::AddComponent(ObjectID entity_id, ComponentTypeID type)
{
    LASSERT(!HasComponent(entity_id, type), "Cannot add duplicate component!");
    const auto& comp_data =  GetCompDataOfType(type);
    auto it = comp_data->ReserveIndex(entity_id);
    ComponentRegistry::GetComponentInfo(type).constructor_where(*it,ID,entity_id);

    GetEntityData(entity_id)->RegisterComponent(type);
    return (IComponentData*)*it;
}

bool Scene::HasComponent(ObjectID entity_id, ComponentTypeID type)
{
    return GetCompDataOfType(type)->Contains(entity_id);
}

std::shared_ptr<data_allocator> Scene::GetCompDataOfType(ComponentTypeID type)
{
    if (!components_all.contains(type))
    {
        const Component_Info_Extended comp_info = ComponentRegistry::GetComponentInfo(type);
        data_allocator::DataProperties data_properties;
        data_properties.copy_func = comp_info.copy_construct;
        data_properties.move_func = comp_info.move_construct;
        data_properties.deconstruct_func = comp_info.deconstructor;
        components_all.emplace(type, std::make_shared<data_allocator>( comp_info.size));
    }
    return components_all[type];
}

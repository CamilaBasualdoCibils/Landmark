﻿#pragma once
#include "Entity.h"
#include "IComponent.h"
//#include <Containers/AllocVector_old.h>
#include <Debug/Logging/Log.h>

#include "EditorTools/EditorEntityList.h"

#include "Debug/Editor.h"
class  EntityManager
{

	static inline EntityStorage entities;
	static inline Log logger = Log("EntityManager");
	
	static void NoEntityLogOutput(EntityID ID);

	static inline std::unordered_map<ComponentTypeID, void*> componentContainers_2;
	
	//static inline std::map<std::string, void*> componentContainers;
	static inline EditorEntityList& EntityList = Editor::GetMainToolGroup().GetOrPushGroup("Windows").PushObject<EditorEntityList>(0);
public:

	static  const EntityStorage& GetEntities() { return entities; }
	static EntityHandle CreateEntity(const std::string& name);

	static EntityHandle GetEntity(EntityID ID);

	static bool ExistsEntity(EntityID id);

	
	COMP_TEMP_FUNC_HEADER
		static constexpr ComponentStorageType<T>& ComponentStorage()
	{

		if constexpr (!std::is_base_of_v<IComponentRegister<T>,T>)
		{
			logger.Critical(std::string(typeid(T).name()) + " Does not extend from IComponentRegister<T> publicly\n");
			//logger.Critical() << typeid(T).name() << " Does not extend from IComponentRegister<T> publicly" << Log::end;
			
		}
		else
		{
			const ComponentTypeID ID = T::GetTypeID();
			if (!componentContainers_2.contains(ID))
			{
				ComponentStorageType<T>* newC = new ComponentStorageType<T>();
				componentContainers_2[ID] = newC;
			}

			return *reinterpret_cast<ComponentStorageType<T>*>( componentContainers_2[ID]);
		}
	}

	COMP_TEMP_FUNC_HEADER
		static CompHandle<T> GetComponent(const EntityID& e)
	{
		if (!ExistsEntity(e))
		{
			NoEntityLogOutput(e);
			return CompHandle<T>();
		}
		auto& storage = ComponentStorage<T>();

		return storage.find(e);
	}
	COMP_TEMP_FUNC_HEADER
		static CompHandle<T> GetComponent(const EntityHandle& e)
	{
		return GetComponent<T>(e->ID);
	}
	static IComponentData* GetComponent(const EntityID& e,const ComponentTypeID& type_id) {
		char* data = static_cast<char*>(
		reinterpret_cast<ObjectAllocator_new<char>*>( componentContainers_2[type_id])->data());
		data += e*ComponentRegistry::GetComponentDesc(type_id).size;
		return reinterpret_cast<IComponentData*>(data);
	}

	COMP_TEMP_FUNC_HEADER
		static bool HasComponent(const EntityID& e)
	{
		if (!ExistsEntity(e))
		{
			NoEntityLogOutput(e);
			return false;
		}
		return ComponentStorage<T>().Contains(e);
	}
	COMP_TEMP_FUNC_HEADER
		static bool HasComponent(const EntityHandle& e)
	{
		return HasComponent<T>(e->ID);
	}

	COMP_TEMP_FUNC_HEADER
		static CompHandle<T> AddComponent(const EntityID& e)
	{
		if (!ExistsEntity(e))
		{
			NoEntityLogOutput(e);
			return CompHandle<T>();
		}
		auto& compStr = ComponentStorage<T>();
		if (compStr.Contains(e))
		{
			EntityHandle ent = EntityManager::GetEntity(e);
			logger.Error("Entity ID (" + std::to_string(e) + ")(" + ent->Name + ") already contains component of type (" + typeid(T).name() + ").");
			//logger.Error() << "Entity ID (" << e << ")(" << ent->Name << ") already contains component of type (" << typeid(T).name() << ")." << Log::end;
			return CompHandle<T>();
		}
		compStr.Insert_Emplace(e,e);
		EntityManager::GetEntity(e)->Components.insert(T::GetTypeID());
		//ComponentStorage<T>().Insert(e, T());

		logger.Debug(std::string(typeid(T).name()) + "Component Added to Entity ID(" + std::to_string(e) + ")(" + entities[e].Name + ")");
		//logger << typeid(T).name() << " Component Added to Entity ID (" << e << ")(" << entities[e].Name << ")" << Log::end;

		return compStr.find(e);


	}
	COMP_TEMP_FUNC_HEADER
		static CompHandle<T> AddComponent(const EntityHandle& e)
	{
		return AddComponent<T>(e->ID);


	}
};
inline void EntityManager::NoEntityLogOutput(EntityID ID)
{
	logger.Error("Entity ID (" + std::to_string(ID) + ") Does not exist");
	//logger.Error() << "Entity ID (" << ID << ") does not exist!" << Log::end;
}

inline EntityHandle EntityManager::CreateEntity(const std::string& name)
{

	auto ptr = entities.Push_Emplace(-1, name);
	const_cast<EntityID&>((*ptr).ID) = ptr.GetIndex();
	(*ptr).Enabled = true;
	return ptr;
}

inline EntityHandle EntityManager::GetEntity(EntityID ID)
{
	if (!ExistsEntity(ID))
	{
		logger.Error("Entity ID (" + std::to_string(ID) + ") does not exist!");
		//logger.Error() << "Entity ID (" << ID << ") does not exist!" << Log::end;
		return EntityHandle();
	}
	return entities.find(ID);
}

inline bool EntityManager::ExistsEntity(EntityID id)
{
	return entities.Contains(id);
}

COMP_TEMP_FUNC_HEADER
CompHandle<T> Entity::AddComponent()
{
	return EntityManager::AddComponent<T>(ID);
}

COMP_TEMP_FUNC_HEADER
CompHandle<T> Entity::GetComponent()
{
	return EntityManager::GetComponent<T>(ID);
}

COMP_TEMP_FUNC_HEADER
bool Entity::HasComponent()
{
	return EntityManager::HasComponent<T>(ID);
}


#pragma once
#include "pch.h"
#include "IComponent.h"
#include "ComponentRegistry.h"
#include "Containers/ObjectAllocator.h"
#include "Containers/ObjectAllocator-new.h"
#include <vector>
#define COMP_TEMP_FUNC_HEADER template <typename T>

struct Entity
{
	Entity(EntityID _id,const std::string& _Name = "") :ID(_id),Name(_Name) {}
	const EntityID ID = -1;
	std::string Name = "Default Entity Name";
	bool Enabled = true;
	std::unordered_set<ComponentTypeID> Components;

	COMP_TEMP_FUNC_HEADER
	CompHandle<T> GetComponent();
	COMP_TEMP_FUNC_HEADER
	CompHandle<T> AddComponent();
	COMP_TEMP_FUNC_HEADER
	bool HasComponent();
private:
	//std::vector<ComponentTypeID> components;
};
using EntityStorage = ObjectAllocator_new<Entity, EntityID,false>;
//using EntityStorage = AllocVector<Entity>;
using Alloc_EntityPtr = EntityStorage::iterator;



using EntityHandle = ObjectHandle<Entity>;
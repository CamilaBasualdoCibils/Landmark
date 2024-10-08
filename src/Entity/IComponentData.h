#pragma once
#include "pch.h"
//#include <Containers/AllocVector_old.h>

#include "Containers/ObjectAllocator.h"
#include "Containers/ObjectAllocator-new.h"
using EntityID = uint32_t;

template <typename T>
using ComponentStorageType = ObjectAllocator_new<T, EntityID, false>;

template <typename T>
using ComponentStorageIterator = typename ComponentStorageType<T>::iterator;

template <typename T>
class ObjectHandle
{
	ComponentStorageIterator<T> iterator;
	bool Valid = false;
public:
	ObjectHandle(const ComponentStorageIterator<T>& o) :iterator(o) {
		Valid = true;
	}
	ObjectHandle() :iterator() {}
	T* operator->() { return &((*iterator)); }
	const T* operator->() const { return  &((*iterator)); }

	constexpr bool isValid() const { return Valid; }
	//T& operator()() { return *allocVectorPtr; }
	//const T& operator()() const { return *allocVectorPtr; }
};


template <typename T>
using CompHandle = ObjectHandle<T>;

//raw data
struct IComponentData
{
	const EntityID ID;
protected:
	bool Enabled = true;
	
	IComponentData(EntityID id) : ID(id) {};
	virtual ~IComponentData() {};
};





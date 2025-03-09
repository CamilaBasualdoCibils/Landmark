#pragma once
#include <pch.h>
#include <Types/ObjectAllocator-new.h>
using ObjectID = uint32_t;
using ComponentTypeID = uint32_t;
using SceneID = ObjectID;
#define ENTITY_COMP_TEMP_FUNC_HEADER template <typename T>

template <typename T>
using ComponentStorageType = ObjectAllocator_new<T, ObjectID, false>;

template <typename T>
using ComponentStorageIterator = typename ComponentStorageType<T>::iterator;
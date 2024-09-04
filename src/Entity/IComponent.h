#ifndef _ICOMPONENT_H_
#define _ICOMPONENT_H_

#include "pch.h"
#include "IComponentData.h"
#include "IComponentRegister.h"

template <typename T>
struct IComponent : public IComponentData, public IComponentRegister<T>
{
	explicit IComponent(EntityID id)
		: IComponentData(id)
	{
	}
	
};
#endif

#ifndef _ICOMPONENTREGISTER_H_
#define _ICOMPONENTREGISTER_H_

#include "pch.h"
//ID assigner
#include "IComponentData.h"
#include "ComponentRegistry.h"
template <typename T>
struct IComponentRegister
{
	private:

	static inline const ComponentRegistry::ComponentDescription description = {
		typeid(T).name(),
		sizeof(T),
		T::ComponentInspector};
	static inline const ComponentTypeID typeID = ComponentRegistry::Register(description);
	public:
	static ComponentTypeID GetTypeID() {return typeID;}
	static size_t GetSize() {return typeID;}

	static void ComponentInspector(IComponentData* comp) {
		ImGui::Text("Inspector for Component not implemented");
	}
	//ComponentTypeID GetTypeID() const {return typeID;}
};
#endif
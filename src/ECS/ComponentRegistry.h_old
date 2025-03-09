#include "pch.h"
#ifndef _COMP_REGISTRY_H_
#define _COMP_REGISTRY_H_
#include "IComponentData.h"
using ComponentTypeID = int32_t;

class ComponentRegistry {

protected:
using ImguiComponentInspector = std::function<void(IComponentData*)>;
	struct ComponentDescription {
		std::string Name;
		size_t size=0;
		ImguiComponentInspector inspectorFunction=nullptr;
	};

	private:
	template <typename T>
	friend class IComponentRegister;
	static inline ComponentTypeID NextComponentID = 0;
	static inline std::unordered_map<ComponentTypeID,ComponentDescription> ComponentDescriptions;
	protected:

	
	static ComponentTypeID Register(const ComponentDescription& desc) {
		LASSERT(desc.size >=sizeof(IComponentData),"Invalid Size for Component");
		

		ComponentTypeID id = NextComponentID++;
		ComponentDescriptions[id] = desc;
		return id;
	}



	public:
	static ComponentDescription GetComponentDesc(ComponentTypeID id) {
		LASSERT(ComponentDescriptions.contains(id),"Component has not been registered");
		return ComponentDescriptions[id];
	}

};
#endif
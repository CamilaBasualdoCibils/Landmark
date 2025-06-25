#pragma once
#include <pch.h>
#include "ComponentInfo.hpp"
class ComponentRegistry
{
    inline static std::size_t nextID = 1; //0 IS RESERVED FOR NULL
    inline static std::vector<Component_Info_Extended> registered_components;

public:
    template <typename T>
    static const Component_Info_Extended &GetComponentInfo();
    static const Component_Info_Extended &GetComponentInfo(ComponentTypeID type);
    template <typename T>
    static ComponentTypeID GetComponentID();
    template <typename T>
    static ComponentTypeID RegisterComponent();
    constexpr static const decltype(registered_components)& GetRegisteredComponents() {return registered_components;}
private:
};

template <typename T>
inline const Component_Info_Extended &ComponentRegistry::GetComponentInfo()
{
    const ComponentTypeID comp_id = GetComponentID<T>();
    LASSERT(registered_components.size() > comp_id, "Component Not Registered!");
}

template <typename T>
inline ComponentTypeID ComponentRegistry::GetComponentID()
{
    static ComponentTypeID id = nextID++;
    return id;
}

template <typename T>
inline ComponentTypeID ComponentRegistry::RegisterComponent()
{
    std::cerr << "Registering" <<typeid(T).name() << std::endl;
    const ComponentTypeID id = GetComponentID<T>();
    if (registered_components.size() <= id)
    {
        registered_components.resize(id + 1);
        registered_components[id] = Component_Info_Extended::FromComponentInfo<T>(T::GetComponentTypeInfo());
    }

    return id;
}

#include "ComponentRegistry.hpp"

const Component_Info_Extended &ComponentRegistry::GetComponentInfo(ComponentTypeID type)
{
    return registered_components[type];
}
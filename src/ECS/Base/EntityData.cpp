#include "EntityData.hpp"

void EntityData::RegisterComponent(ComponentTypeID type_id)
{
    if (component_count == MAX_COMPONENT_COUNT) throw "Max component count reached";

    if (std::find(components.begin(),components.end(),type_id)!= components.end()) 
        throw "Cannot have duplicate component";

    auto pos =std::find(components.begin(),components.end(),0);
    *pos = type_id;
    component_count++;

    
   
}

void EntityData::DeregisterComponent(ComponentTypeID type_id)
{
    auto pos =std::find(components.begin(),components.end(),type_id);
    *pos = 0;
    component_count--;
    
    throw "Component Not Found";
}

bool EntityData::Has_Component(ComponentTypeID type_id)
{
    auto pos =std::find(components.begin(),components.end(),type_id);
    return pos != components.end();
}

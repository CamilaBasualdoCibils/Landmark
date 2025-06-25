#pragma once
#include <ECS/Defines.hpp>

struct Object
{
    Object(ObjectID id) : ID(id) {}
    ObjectID ID;
};

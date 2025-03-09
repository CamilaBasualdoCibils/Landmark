#pragma once
#include <ECS/Defines.hpp>
struct Object
{
    Object(ObjectID id) : ID(id) {}
    ObjectID ID;
};

struct SceneObject : Object
{

    SceneID scene_id;

    SceneObject(SceneID scene, ObjectID id) : Object(id), scene_id(scene)
    {
    }

    
};
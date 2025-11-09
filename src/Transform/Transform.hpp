#pragma once
#include <Entity/IComponent.hpp>
#include <pch.h>
#include "Editor/EditorTemplates.hpp"
struct Transform
{
    vec3 Position = {0, 0, 0};
    quat Rotation = glm::quat(vec3(0, 0, 0));
    vec3 Scale = {1, 1, 1};

    mat4 GetModelMatrix() const
    {
        mat4 T = translate(identity<mat4>(), Position);
        mat4 R(Rotation);
        mat4 S = glm::scale(identity<mat4>(), Scale);
        return T * R * S;
    }

    constexpr vec3 Forward() const {return Rotation * vec3(0,0,1);}
    constexpr vec3 Up() const {return Rotation * vec3(0,1,0);}
    constexpr vec3 Right() const {return Rotation * vec3(-1,0,0);}
};


#pragma once
#include "Editor/EditorTemplates.hpp"
#include "Mesh.hpp"
#include <pch.h>
struct EntityInfo
{
    std::string Name = "Default Name";
    bool Enabled = true;
};
struct Transform
{
    vec3 Position = {0, 0, 0};
    quat Rotation = glm::quat(vec3(0, 0, 0));
    vec3 Scale = {1, 1, 1};
    void DrawInspector()
    {
        Editor::DragVec3("Position", &Position, 0.1f);
        Editor::Rotation("Rotation", &Rotation);
        Editor::DragVec3("Scale", &Scale, 0.1f);
    }
    mat4 GetModelMatrix() const
    {
        mat4 T = translate(identity<mat4>(), Position);
        mat4 R(Rotation);
        mat4 S = glm::scale(identity<mat4>(), Scale);
        return T * R * S;
    }
};

struct MeshDescriptor
{
    std::shared_ptr<Mesh> mesh;
};

struct Camera
{
    enum class ProjectionModes
    {
        PERSPECTIVE,
        INF_PERSPECTIVE,
        ORTHOGRAPHIC,
        INF_ORTHOGRAPHIC
    };
    // Horizontal
    float FOV = 70;
    float SensorSize = 1;
    vec2 ZclipPlanes = {0.1, 100};
    float AspectRatio = 1;
    bool LogarithmicDepth = true;
    ProjectionModes projectionMode = ProjectionModes::PERSPECTIVE;

    mat4 GetViewMatrix(const Transform &t) const
    {
        // TODO : IMPROVE THIS vvvvv
        return glm::inverse(t.GetModelMatrix());
    }
    mat4 GetProjMatrix() const
    {

        mat4 mat;
        switch (projectionMode)
        {
        case ProjectionModes::PERSPECTIVE:
            mat = glm::perspective(FOV, AspectRatio, ZclipPlanes.x, ZclipPlanes.y);
            break;
        case ProjectionModes::INF_PERSPECTIVE:

            mat = infinitePerspective(FOV, AspectRatio, ZclipPlanes.x);
            break;
        case ProjectionModes::ORTHOGRAPHIC:
            mat = glm::ortho(SensorSize, SensorSize, SensorSize, SensorSize);
            // logger.Critical("Implement this you fucking idiot");
            // logger.Critical() << "Implement this you fucking idiot" << Log::end;
            break;
        case ProjectionModes::INF_ORTHOGRAPHIC:
            mat = glm::identity<mat4>();
            // logger.Critical() << "Implement this you fucking idiot" << Log::end;
            break;
        }
        mat[1][1] *= -1; // VULKAN Y AXIS IS DOWN. THIS MAKES IT GO UP

        return mat;
    }
};

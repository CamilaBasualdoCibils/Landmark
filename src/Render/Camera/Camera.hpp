#pragma once
#include <pch.h>

#include "Transform/Transform.hpp"
struct Camera
{
    Camera();
    ~Camera();

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
    mat4 GetViewMatrix(const Transform &t) const;
    mat4 GetProjMatrix() const;
};

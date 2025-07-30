#include "Camera.hpp"
#include "Graphics/Compositor/Compositor.hpp"
#include "Render/Components/Entity.hpp"
Camera::Camera()
{
    compositeGroup = Graphics::Compositor::Get().MakeRootGroup(
        Graphics::CompositeGroupProperties{.Name = "Camera Composite", .ResolutionOverride = uvec2{1920, 1080}});
}

Camera::~Camera()
{
}

mat4 Camera::GetViewMatrix(const Transform &t) const
{
    // TODO : IMPROVE THIS vvvvv
    return glm::inverse(t.GetModelMatrix());
}

mat4 Camera::GetProjMatrix() const
{

    mat4 mat;
    switch (projectionMode)
    {
    case ProjectionModes::PERSPECTIVE:
        mat = glm::perspective(radians(FOV), AspectRatio, ZclipPlanes.x, ZclipPlanes.y);
        break;
    case ProjectionModes::INF_PERSPECTIVE:

        mat = infinitePerspective(radians(FOV), AspectRatio, ZclipPlanes.x);
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


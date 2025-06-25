#include "CameraComponent.hpp"
#include <Transform/TransformComponent.hpp>
void CameraComponent::DrawInspector()
{
    if (ImGui::RadioButton("Perspective", GetProjectionMode() == Camera::ProjectionModes::PERSPECTIVE))
        SetProjectionMode(Camera::ProjectionModes::PERSPECTIVE);
    ImGui::SameLine();
    if (ImGui::RadioButton("Orthographic", GetProjectionMode() == Camera::ProjectionModes::ORTHOGRAPHIC))
        SetProjectionMode(Camera::ProjectionModes::ORTHOGRAPHIC);
    if (ImGui::RadioButton("Inf Perspective", GetProjectionMode() == Camera::ProjectionModes::INF_PERSPECTIVE))
        SetProjectionMode(Camera::ProjectionModes::INF_PERSPECTIVE);
    ImGui::SameLine();
    if (ImGui::RadioButton("Inf Orthographic", GetProjectionMode() == Camera::ProjectionModes::INF_ORTHOGRAPHIC))
        SetProjectionMode(Camera::ProjectionModes::INF_ORTHOGRAPHIC);

    const bool perspective = GetProjectionMode() == Camera::ProjectionModes::PERSPECTIVE ||
    GetProjectionMode() == Camera::ProjectionModes::INF_PERSPECTIVE;
    const bool infinite =  GetProjectionMode() == Camera::ProjectionModes::INF_PERSPECTIVE ||GetProjectionMode() == Camera::ProjectionModes::INF_ORTHOGRAPHIC;
    if (perspective) {
        float FOV = GetFOV();
        if (ImGui::SliderFloat("FOV",&FOV,0,360))
        SetFOV(FOV);

        

    }
    else {
        float sensize = _SensorSize;
        if (ImGui::DragFloat("Sensor Size",&_SensorSize))
            SetSensorSize(sensize);
    }
    vec2 zplanes = GetZPlanes();
    if (!infinite) {    
        ImGui::DragFloat2("Z planes",&zplanes.x,0.05);
    }
    else ImGui::DragFloat("Near Plane",&zplanes[0],0.05);
    SetZPlanes(zplanes);
}

const mat4 &CameraComponent::GetViewMatrix()
{    
    const mat4& model_matrix = GetComponent<TransformComponent>()->GetModelMatrix();
    if (model_matrix != last_model_matrix) {
        viewMatrix = inverse(model_matrix);
        last_model_matrix = model_matrix;
    }
    return viewMatrix;
}

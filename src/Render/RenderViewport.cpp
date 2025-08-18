#include "RenderViewport.hpp"
#include "World.hpp"

#include "Components/CameraComponent.hpp"
#include "Components/TransformComponent.hpp"
#include "Editor/EditorLambdaItem.hpp"
#include "Graphics/Compositor/CompositeGroup.hpp"
#include "Render/World.hpp"
#include "misc/Conversions.hpp"
#include "EditorViewRenderer.hpp"
void RenderViewport::DrawWindowContents()
{
    Entity &RenderCamera = SelectedCamera.has_value() ? *SelectedCamera : ViewportCameraEntity;
    ivec2 AvailableSpace = glm::max(vec2(0.0f), ImGuiToGlm(ImGui::GetContentRegionAvail()));

    std::shared_ptr<Graphics::CompositeGroup> cameraCompositeGroup =
        RenderCamera.GetComponent<CameraComponent>().compositeGroup;
    cameraCompositeGroup->SetResolutionOverride(AvailableSpace);
    GPURef<VK::Texture> ColorImage = cameraCompositeGroup->GetAttachments().at("Color");
    uvec2 ImageResolution = (uvec2)ColorImage->GetImage()->GetProperties().Dimensions;
    const vec2 MaxUvThatFits = (vec2)AvailableSpace / (vec2) ImageResolution;
    ImGui::Image((ImTextureID)ColorImage->GetImguiDescriptorSet(),
                 GlmToImGui(AvailableSpace),ImVec2(0,0),GlmToImGui(MaxUvThatFits));

    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Right))
    {

        CameraComponent &camera = RenderCamera.GetComponent<CameraComponent>();
        TransformComponent &transform = RenderCamera.GetComponent<TransformComponent>();
        static vec2 InitialMousePos;
        static quat InitialRot;
        static vec3 InitialRotEuler;

        if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
        {
            InitialMousePos = ImGuiToGlm(ImGui::GetMousePos());
            InitialRot = transform.Rotation;
            InitialRotEuler = degrees(eulerAngles(InitialRot));
        }
        const vec3 MovementVector = {
            ImGui::IsKeyDown(ImGuiKey_A) * -1 + ImGui::IsKeyDown(ImGuiKey_D),
            ImGui::IsKeyDown(ImGuiKey_LeftShift) * -1 + ImGui::IsKeyDown(ImGuiKey_Space),
            ImGui::IsKeyDown(ImGuiKey_W) * -1 + ImGui::IsKeyDown(ImGuiKey_S),
        };
        transform.Position += transform.Rotation * (MovementVector * (float)World::Get().GetDeltaTime());

        vec2 MouseDelta = ImGuiToGlm(ImGui::GetMousePos()) - InitialMousePos;

        MouseDelta /= ImGui::GetItemRectSize().x;
        MouseDelta *= 360.0f;

        float newYaw = InitialRotEuler.y - MouseDelta.x;
        float newPitch =
            glm::clamp(InitialRotEuler.x - MouseDelta.y, -89.0f, 89.0f); // Clamp pitch to avoid gimbal lock

        quat qPitch = glm::angleAxis(glm::radians(newPitch), vec3(1, 0, 0));
        quat qYaw = glm::angleAxis(glm::radians(newYaw), vec3(0, 1, 0));

        transform.Rotation = qYaw * qPitch; // Yaw then pitch

        camera.FOV = glm::clamp(((ImGui::IsKeyDown(ImGuiKey_C) * -1 + ImGui::IsKeyDown(ImGuiKey_Z))) *
                                        (float)World::Get().GetDeltaTime() * 30.0f +
                                    camera.FOV,
                                30.0f, 150.0f);
    }
}

RenderViewport::RenderViewport()
    : Editor::ToolItem("Viewport"), Editor::Window<>("Viewport"), ViewportCameraEntity(World::Get().CreateEntity())
{
    ViewportCameraEntity.AddOrGetComponent<CameraComponent>();
    ViewportCameraEntity.GetInfo().Name = "Viewport Camera";
    ViewportCameraEntity.GetComponent<CameraComponent>().compositeGroup->AddLayer<EditorViewRenderer>(ViewportCameraEntity);
    ViewportCameraEntity.GetInfo().Locked = true;
    GetMenuBar().PushObject<Editor::LambdaItem>(0, "Camera Selector", [&]() {
        if (ImGui::BeginMenu("Camera"))
        {
            auto CameraView = World::Get().GetComponentView<CameraComponent, EntityInfo>();

            for (auto Ent : CameraView)
            {
                const EntityInfo &info = CameraView.get<EntityInfo>(Ent);
                if (info.Hidden)
                    continue;
                bool selected = SelectedCamera.has_value() && SelectedCamera->GetID() == Ent;
                if (ImGui::MenuItem(info.Name.c_str(), nullptr, &selected))
                {
                    if (selected)
                    {
                        SelectedCamera = Entity(Ent);
                    }
                    else
                    {
                        SelectedCamera.reset();
                    }
                }
            }
            ImGui::EndMenu();
        }
    });
}

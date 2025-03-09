#include "Renderer.hpp"
#include "Core/App.h"
#include "IO/IO.h"
#include <Debug/Editor.h>
#include <Renderer/EditorTools/EditorViewport.h>
#include <Transform/Transform.h>
#include "Camera.h"

#include "Components/CameraComponent.hpp"
#include <Transform/TransformComponent.hpp>
#include <ECS/ECS.hpp>
std::vector<AppModule::EngineCallInject> Renderer::GetInjections()
{
    std::vector<EngineCallInject> injections;

    AppModule::EngineCallInject render_begin("Renderer render_begin");
    render_begin.call = std::bind(&Renderer::RenderBegin, this);
    render_begin.call_point = AppModule::EngineCallPoints::RENDER;
    render_begin.priority = 50;
    injections.push_back(render_begin);

    AppModule::EngineCallInject render_exit("Renderer render_end");
    render_exit.call = std::bind(&Renderer::RenderEnd, this);
    render_exit.call_point = AppModule::EngineCallPoints::RENDER;
    render_exit.priority = -50;
    injections.push_back(render_exit);

    return injections;
}

Renderer::Renderer()
{
    ComponentRegistry::RegisterComponent<CameraComponent>();
    ComponentRegistry::RegisterComponent<TransformComponent>();
}
Component<CameraComponent> camera_comp;
void Renderer::Init()
{

    auto scene = App::GetInstance()->GetModule<SceneManager>()->CreateScene("TestScene");
    Entity cube = scene->CreateEntity("Cube");
    cube.AddComponent<TransformComponent>();
    Entity camera = scene->CreateEntity("Camera");
    camera.AddComponent<TransformComponent>();
    camera_comp = camera.AddComponent<CameraComponent>();


    window = App::GetInstance()->GetMainWindow();
    main_device = App::GetInstance()->GetMainDevice();

    schedule_properties sched_prop;
    sched_prop.rendering_queue = &main_device->GetGraphicsQueue();
    sched_prop.presention_queue = &main_device->GetPresentQueue();
    schedule = std::make_shared<Schedule>(sched_prop);

    stage_properties stg_prop;
    stg_prop.vertex_parameters = {{"in_position", StageInstanceDataType::FLOAT3}};
    stg_prop.actor_parameters = {
        {"m0", StageInstanceDataType::FLOAT4},
        {"m1", StageInstanceDataType::FLOAT4},
        {"m2", StageInstanceDataType::FLOAT4},
        {"m3", StageInstanceDataType::FLOAT4},
    };
    stage = std::make_shared<Stage>(stg_prop);

    act_properties act_prop;
    RenderPass::attachment attch;
    attch.format = Format::BGRA8unorm;
    attch.finalLayout = ImageLayouts::SHADER_READ_ONLY;
    act_prop.color_attachments = {attch};
    scene_act = std::make_shared<Act>(act_prop);

    attch.format = Format::BGRA8unorm;
    attch.finalLayout = ImageLayouts::PRESENT_SRC;
    act_prop.color_attachments = {attch};
    imgui_act = std::make_shared<Act>(act_prop);

    lens_properties lens_prop;
    Folder shader_folder = IO::GetResources().GetFolder("shaders");
    lens_prop.shaderStages[ShaderStage::VERTEX] = shader_folder.GetFile("test.vert.spv").GetContent_Binary();
    lens_prop.shaderStages[ShaderStage::FRAGMENT] = shader_folder.GetFile("test.frag.spv").GetContent_Binary();
    lens_prop.cull_mode = CullModes::BACK;
    lens = std::make_shared<Lens>(lens_prop, *stage, *scene_act);

    film_properties proj_prop;
    proj_prop.present_mode = PresentModes::IMMEDIATE;
    proj_prop.surface = window->GetSurface();
    film = std::make_shared<Film>(proj_prop, *imgui_act);
    frame_properties frame_prop;
    frame_prop.dimensions = {1000, 1000, 1};
    frame_prop.lens_subpass_id = 0;
    scene_frame = std::make_shared<Frame>(frame_prop, *scene_act);
    imgui.Init(*App::GetInstance()->GetMainInstance(),
               *main_device, main_device->GetGraphicsQueue(),
               imgui_act->getRenderPass(),
               *window,
               imgui_act->GetProperties().color_attachments[0].format);

    const float v_size_x = 1.0f, v_size_y = 1.0f, v_size_z = 1.0f;
    vec3 vertices[] = {
        // Front face
        {-v_size_x, -v_size_y, v_size_z}, // 0
        {v_size_x, -v_size_y, v_size_z},  // 1
        {v_size_x, v_size_y, v_size_z},   // 2
        {-v_size_x, v_size_y, v_size_z},  // 3

        // Back face
        {-v_size_x, -v_size_y, -v_size_z}, // 4
        {v_size_x, -v_size_y, -v_size_z},  // 5
        {v_size_x, v_size_y, -v_size_z},   // 6
        {-v_size_x, v_size_y, -v_size_z}   // 7
    };

    uint32_t indices[] = {
        1, 0, 2,
        3, 2, 0,
        4, 5, 7,
        6, 7, 5,
        0, 4, 3,
        7, 3, 4,
        5, 1, 6,
        2, 6, 1,
        2, 3, 6,
        7, 6, 3,
        5, 4, 1,
        0, 1, 4};
    std::span<vec3> v_span = vertices;
    std::span<uint32_t> i_span = indices;
    stage->InsertVertexData(v_span, 0);

    stage->InsertIndexData(i_span, 0);
    editor_viewport = &Editor::GetMainToolGroup().GetOrPushGroup("Windows").PushObject<EditorViewport>(0, this);
}
void Renderer::RenderBegin()
{
    imgui.NewFrame();

    const float distance = 5.0f;
    vec3 pos = glm::normalize(vec3(sinf((float)glfwGetTime()), cosf((float)glfwGetTime() * 0.33f), cosf((float)glfwGetTime())));

    Transform cam_trans;
    cam_trans.Position() = pos * distance;
    cam_trans.Rotation() = quatLookAt(-pos, vec3(0, 1, 0));

    const Viewport &current_viewport = editor_viewport->getViewport();
    camera_comp->GetCamera().SetAspectRatio(current_viewport.size.x / current_viewport.size.y);
    static bool inf = false;
    ImGui::Begin("Camera");
    ImGui::Checkbox("Inf_test", &inf);
    ImGui::End();
    mat4 matrix = camera_comp->GetCamera().GetProjMatrix() * camera_comp->GetCamera().GetViewMatrix(cam_trans);
    std::span<mat4> sp = {&matrix, &matrix + 1};
    stage->InsertActorData(sp, 0);

    Editor::Draw();
    imgui.EndFrame();

    // schedule->SetViewport()
    schedule->PrepareNextFrame(*film);
    schedule->Begin();
    schedule->PrepareStage(*stage);
    schedule->UseLens(*lens)
        .BeginAct(*scene_frame, *scene_act)
        .FullViewport()
        .FullScissor();
    schedule->Capture(*stage, {});
    schedule->EndAct();
    schedule->BeginAct(*film, *imgui_act);
}

void Renderer::RenderEnd()
{

    imgui.Render(schedule->GetReadyCmdBuffer());
    schedule->EndAct();
    schedule->End();
    schedule->Execute();
    schedule->Present(*film);
    schedule->WaitUntilIdle();
    schedule->Clear();
}

#include "Renderer.hpp"
#include "Core/App.h"
#include "IO/IO.h"
#include <Editor/Editor.h>
#include <Renderer/EditorTools/EditorViewport.h>
#include <Transform/Transform.h>
#include "Camera.h"

#include "Components/CameraComponent.hpp"
#include <Transform/TransformComponent.hpp>
#include <ECS/ECS.hpp>
#include <VK/Shaders/Shader.hpp>

Folder shader_folder = IO::GetFolder("assets").GetFolder("shaders");
std::string vert_source = shader_folder.GetFile("test.vert").ReadText();
std::string frag_source = shader_folder.GetFile("test.frag").ReadText();

class ShaderEditorTestTool : EditorTool
{
public:
    ShaderEditorTestTool() : EditorTool("Shader Edit Test") {}
    void DrawWindowContents() override
    {
        ImGui::Text("Hello");
        auto region_available = ImGui::GetContentRegionAvail();
        region_available.y /= 2;
        bool modified = ImGui::InputTextMultiline("##Vertex", &vert_source,
                                                  region_available, ImGuiInputTextFlags_AllowTabInput);
        modified |= ImGui::InputTextMultiline("##Fragment", &frag_source,
                                              region_available, ImGuiInputTextFlags_AllowTabInput);
        if (modified)
        {

            static Shader shader = Shader()
                                       .SetSourceType(ShaderSourceType::GLSL)
                                       .SetClient(ShaderClientType::VULKAN_1_4)
                                       .SetTargetSpv(ShaderSpvVersion::Spv_1_6);

            ShaderCompileResult vert_compile = shader.SetType(ShaderType::VERTEX)
                                                   .SetSource({vert_source})
                                                   .Compile();
            ShaderCompileResult frag_compile = shader.SetType(ShaderType::FRAGMENT)
                                                   .SetSource({frag_source})
                                                   .Compile();
            if (!vert_compile.compile_successful || !frag_compile.compile_successful)
            {
                if (!vert_compile.compile_successful)
                {
                    std::cout << vert_compile.compile_log << std::endl;
                }
                if (!frag_compile.compile_successful)
                {
                    std::cout << frag_compile.compile_log << std::endl;
                }
                return;
            }
            auto renderer = App::GetInstance()->GetModule<Renderer>();
            auto &lens = renderer->GetSceneLens();
            lens.reset();
            lens_properties lens_prop;
            lens_prop.shaderStages[ShaderStage::VERTEX] = vert_compile.data;
            lens_prop.shaderStages[ShaderStage::FRAGMENT] = frag_compile.data;
            // lens_prop.shaderStages[ShaderStage::VERTEX] = shader_folder.GetFile("test.vert.spv").ReadBinary();
            // lens_prop.shaderStages[ShaderStage::FRAGMENT] = shader_folder.GetFile("test.frag.spv").ReadBinary();
            lens_prop.cull_mode = CullModes::BACK;
            lens = std::make_shared<Lens>(lens_prop, *renderer->GetSceneStage(), *renderer->GetSceneAct());
        }
    }
    bool open = false;
    void DrawHandle() override
    {
        ImGui::MenuItem("Shader Edit Test", 0, &open);
    }

    void DrawTool() override
    {
        if (open && ImGui::Begin("Shader Edit Test", &open, ImGuiWindowFlags_MenuBar))
        {
            Draw();
            ImGui::End();
        }
    }
};

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
Component<TransformComponent> cube_trans;
void Renderer::Init()
{

    auto scene = App::GetInstance()->GetModule<SceneManager>()->CreateScene("TestScene");
    Entity cube = scene->CreateEntity("Cube");
    cube_trans = cube.AddComponent<TransformComponent>();
    Entity camera = scene->CreateEntity("Camera");
    camera.AddComponent<TransformComponent>();
    camera_comp = camera.AddComponent<CameraComponent>();
    scene->CreateEntity("empty entity");

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
    attch.format = Format::BGRA_8_UNORM;
    attch.finalLayout = ImageLayouts::SHADER_READ_ONLY;
    act_prop.color_attachments = {attch};
    scene_act = std::make_shared<Act>(act_prop);

    attch.format = Format::BGRA_8_UNORM;
    attch.finalLayout = ImageLayouts::PRESENT_SRC;
    act_prop.color_attachments = {attch};
    imgui_act = std::make_shared<Act>(act_prop);

    Folder shader_folder = IO::GetFolder("assets").GetFolder("shaders");

    Shader shader = Shader().SetSourceType(ShaderSourceType::GLSL).SetClient(ShaderClientType::VULKAN_1_4).SetTargetSpv(ShaderSpvVersion::Spv_1_6);

    ShaderCompileResult vert_compile = shader.SetType(ShaderType::VERTEX)
                                           .SetSource({shader_folder.GetFile("test.vert").ReadText()})
                                           .Compile();
    ShaderCompileResult frag_compile = shader.SetType(ShaderType::FRAGMENT)
                                           .SetSource({shader_folder.GetFile("test.frag").ReadText()})
                                           .Compile();

    lens_properties lens_prop;
    lens_prop.shaderStages[ShaderStage::VERTEX] = vert_compile.data;
    lens_prop.shaderStages[ShaderStage::FRAGMENT] = frag_compile.data;
    // lens_prop.shaderStages[ShaderStage::VERTEX] = shader_folder.GetFile("test.vert.spv").ReadBinary();
    // lens_prop.shaderStages[ShaderStage::FRAGMENT] = shader_folder.GetFile("test.frag.spv").ReadBinary();
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
        0, 1, 2,
        2, 3, 0,
        5, 4, 7,
        7, 6, 5,
        4, 0, 3,
        3, 7, 4,
        1, 5, 6,
        6, 2, 1,
        3, 2, 6,
        6, 7, 3,
        4, 5, 1,
        1, 0, 4};

    OBJ_File_content obj_file;
    MeshFile::ParseOBJ(IO::GetFolder("assets").GetFolder("models").GetFile("cube.obj"), obj_file);
    std::vector<vec3> positions;
        std::vector<uint32_t> indicies;
    std::for_each(obj_file.verticies.begin(),obj_file.verticies.end(),[&](const OBJ_File_content::Vertex& v){
        positions.push_back(v.position);
    });
    std::for_each(obj_file.indicies.begin(),obj_file.indicies.end(),[&](const uint64_t& v){
        indicies.push_back(v);
    });
        std::span<vec3>v_span = positions;
    std::span<uint32_t> i_span = indicies;

    stage->InsertVertexData(v_span, 0);

    stage->InsertIndexData(i_span, 0);
    editor_viewport = &Editor::GetInstance()->GetMainToolGroup().GetPanelsGroup().PushObject<EditorViewport>(0, this);
    editor_viewport->SetIsOpen(true);
    Editor::GetInstance()->GetMainToolGroup().PushObject<ShaderEditorTestTool>();
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
    camera_comp->SetAspectRatio(current_viewport.size.x / current_viewport.size.y);
    const mat4 &model_mat = cube_trans->GetModelMatrix();
    const mat4 &view_mat = camera_comp->GetViewMatrix();
    const mat4 &proj_mat = camera_comp->GetProjMatrix();
    mat4 matrix = proj_mat * view_mat * model_mat;
    std::span<mat4> sp = {&matrix, &matrix + 1};
    stage->InsertActorData(sp, 0);

    Editor::GetInstance()->Draw();
    imgui.EndFrame();

    // schedule->SetViewport()
    schedule->PrepareNextFrame(*film,*imgui_act);
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

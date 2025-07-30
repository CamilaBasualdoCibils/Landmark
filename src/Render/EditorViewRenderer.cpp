#include "EditorViewRenderer.hpp"
#include "Components/CameraComponent.hpp"
#include "Components/TransformComponent.hpp"
#include "Graphics/Compositor/CompositeContext.hpp"
#include "Graphics/Pipelines/ShaderCompiler.hpp"
#include "Graphics/Vulkan/Commands/CommandManager/VKImageCommands.hpp"
#include "Graphics/Vulkan/Commands/CommandManager/VKPipelineCommands.hpp"
#include "Graphics/Vulkan/Commands/CommandManager/VKRenderingCommands.hpp"
#include "Render/RenderingDefinitions.hpp"
EditorViewRenderer::EditorViewRenderer(std::shared_ptr<Graphics::CompositeGroup> Parent, Entity CameraEntity)
    : ICompositeLayer(Parent, Graphics::CompositeLayerProperties{.Name = "EditorView",
                                                                 .Attachments = Render::PresentStageAttachments}),
      CameraEntity(CameraEntity)
{
    CmdManager.Make();
    RenderTarget.Make();
    RenderTarget->AttachColor(0, GetAttachments().at(Render::ColorAttachmentName));
    RenderTarget->AttachDepth(GetAttachments().at(Render::DepthAttachmentName));

    std::pair<std::vector<std::byte>, std::vector<std::byte>> BinaryVKGL[2];
    const static std::string VertSource = R"(
    #version 460
    layout(location = 0) out vec3 worldPos;
     layout( push_constant ) uniform constants
{
        mat4 InvViewProjMat;
        mat4 ViewProjMat;
        vec3 CameraPosition;

} PushConstants;
    void main() {
        const vec2 pos[3] = vec2[](
            vec2(-1.0, -1.0),
            vec2( 3.0, -1.0),
            vec2(-1.0,  3.0)
        );
        vec2 p = pos[gl_VertexIndex];
        vec4 worldH = PushConstants.InvViewProjMat *vec4(p,0,1.0);
        vec3 world = (worldH/max(worldH.w, 1e-5)).xyz;
        vec3 rayDir = normalize(world - PushConstants.CameraPosition);
        float t = -PushConstants.CameraPosition.y / rayDir.y;
        vec3 intersection = PushConstants.CameraPosition + t * rayDir;
        gl_Position = PushConstants.ViewProjMat * vec4(intersection,1);
        worldPos = intersection;
    })";
    // https://www.shadertoy.com/view/XtBfzz Filtered Grid implementation
    const static std::string FragSource = R"(
    #version 460
    layout(location = 0) out vec4 FragColor;
        layout(location = 0) in vec3 worldPos;
    layout( push_constant ) uniform constants
{
                mat4 InvViewProjMat;
        mat4 ViewProjMat;
        vec3 CameraPosition;

} PushConstants;
    void main() {
    FragColor = vec4(worldPos, 1.0); // Red
    //return;
    //discard;
    })";
    const auto VertResult = Graphics::ShaderCompiler::Get().Compile(
        Graphics::ShaderCompileInfo{.shader_type = Graphics::ShaderType::eVertex,
                                    .source_type = Graphics::ShaderSourceType::eGLSL,
                                    .client_type = Graphics::ShaderClientType::eVulkan_1_4,
                                    .Source = VertSource});

    const auto FragResult = Graphics::ShaderCompiler::Get().Compile(
        Graphics::ShaderCompileInfo{.shader_type = Graphics::ShaderType::eFragment,
                                    .source_type = Graphics::ShaderSourceType::eGLSL,
                                    .client_type = Graphics::ShaderClientType::eVulkan_1_4,
                                    .Source = FragSource});
    if (!FragResult.compile_successful || !VertResult.compile_successful)
    {
        std::cerr << "Frag:\n" << FragResult.compile_log << "\nVert:\n" << VertResult.compile_log;
    }
    GridPipelineLayout = GPURef<VK::PipelineLayout>::MakeRef(
        VK::PipelineLayoutProperties{.PushConstants = {VK::PipelineLayoutPushConstantRange{
                                         .Offset = 0,
                                         .Size = sizeof(GridPushConstants),
                                         .Stages = {VK::ShaderStages::eFragment, VK::ShaderStages::eVertex}}}});
    GridPipeline =
        GPURef<VK::Pipeline>::MakeRef(VK::PipelineProperties{.Layout = GridPipelineLayout,
                                                             .VariantProperties = VK::GraphicsPipelineProperties{
                                                                 .VertexBinary = VertResult.data,
                                                                 .FragmentBinary = FragResult.data,
                                                                 .VertexAttributes = {},
                                                                 .cullMode = VK::CullMode::eNone,
                                                                 .DepthTest = false,
                                                                 .DepthWrite = false,
                                                                 .DepthAttachmentFormat = VK::Format::eDepth32_SFloat,
                                                             }});
}
Graphics::CompositeLayerExecute EditorViewRenderer::OnRender(const Graphics::CompositeContext &Context)
{
    GridPushConstants pushConstants;
    {
        const TransformComponent &CameraTransform = CameraEntity.GetComponent<TransformComponent>();
        const CameraComponent &CameraComp = CameraEntity.GetComponent<CameraComponent>();
        pushConstants.InvViewProjMat = CameraTransform.GetModelMatrix() * glm::inverse(CameraComp.GetProjMatrix());
        pushConstants.ViewProjMat = CameraComp.GetProjMatrix() * CameraComp.GetViewMatrix();
        pushConstants.CameraPosition = CameraTransform.Position;
    }

    CmdManager->Clear();
    CmdManager->PushGroupLabel("Editor View Render");

    const ivec2 Resolution = GetResolution();
    CmdManager->Push(VK::CopyImageCommand(Context.previousLayers.front()->GetAttachment(Render::ColorAttachmentName),
                                          GetAttachment(Render::ColorAttachmentName), VK::ImageLayouts::eGeneral,
                                          VK::ImageLayouts::eGeneral, ivec3(0), ivec3(0), ivec3(Resolution, 1)));
    CmdManager->Push(VK::CopyImageCommand(Context.previousLayers.front()->GetAttachment(Render::DepthAttachmentName),
                                          GetAttachment(Render::DepthAttachmentName), VK::ImageLayouts::eGeneral,
                                          VK::ImageLayouts::eGeneral, ivec3(0), ivec3(0), ivec3(Resolution, 1)));

    CmdManager->Push(VK::PushConstantsCommand(
        GridPipelineLayout, (size_t)0, Flags<VK::ShaderStages>{VK::ShaderStages::eFragment, VK::ShaderStages::eVertex},
        std::vector<GridPushConstants>{pushConstants}));
    RenderTarget->SetViewport({0, 0}, Resolution);
    CmdManager->BeginRendering(RenderTarget);
    CmdManager->Push(VK::BindPipelineCommand(GridPipeline, VK::PipelineBindPoint::eGraphics));
    CmdManager->Push(VK::DrawCommand(3, 1));
    CmdManager->EndRendering();
    CmdManager->PopGroupLabel();

    return Graphics::CompositeLayerExecute{.CommandManagers = {CmdManager}};
}

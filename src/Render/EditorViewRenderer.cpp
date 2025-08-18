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
    layout(location = 0) out vec3 NearPoint;
    layout(location = 1) out vec3 FarPoint;
     layout( push_constant ) uniform constants
{
        mat4 InvViewProjMat;
        mat4 ViewProjMat;
        vec3 CameraPosition;

} PushConstants;
 vec3 UnprojectPoint(float x, float y, float z) {

    vec4 unprojectedPoint =  PushConstants.InvViewProjMat * vec4(x, y, z, 1.0);
    return unprojectedPoint.xyz / unprojectedPoint.w;
}
    void main() {
        const vec2 pos[3] = vec2[](
            vec2(-1.0, -1.0),
            vec2( 3.0, -1.0),
            vec2(-1.0,  3.0)
        );
        vec2 p = pos[gl_VertexIndex];
       NearPoint = UnprojectPoint(p.x, p.y, 0.0).xyz; // unprojecting on the near plane
    FarPoint = UnprojectPoint(p.x, p.y, 1.0).xyz; // unprojecting on the far plane
        gl_Position = vec4(p,0,1);

    }
        )";
    // https://www.shadertoy.com/view/XtBfzz Filtered Grid implementation
    const static std::string FragSource = R"(
    #version 460
    layout(location = 0) in vec3 NearPoint;
    layout(location = 0) out vec4 FragColor;
    layout(location = 1) in vec3 FarPoint;
    layout( push_constant ) uniform constants
{
                mat4 InvViewProjMat;
        mat4 ViewProjMat;
        vec3 CameraPosition;

} PushConstants;
 float gridTextureGradBox( in vec2 p, in vec2 ddx, in vec2 ddy )
{
 float N = 100.0; // grid ratio
	// filter kernel
    vec2 w = max(abs(ddx), abs(ddy)) + 0.01;

	// analytic (box) filtering
    vec2 a = p + 0.5*w;                        
    vec2 b = p - 0.5*w;           
    vec2 i = (floor(a)+min(fract(a)*N,1.0)-
              floor(b)-min(fract(b)*N,1.0))/(N*w);
    //pattern
    return max(0.0,min(1.0,(1.0-i.x)*(1.0-i.y)));
}

    void main() {
     float t = -NearPoint.y / (FarPoint.y - NearPoint.y);
     vec3 fragPos3D = NearPoint + t * (FarPoint - NearPoint);
     vec4 clip_space_pos = PushConstants.ViewProjMat * vec4(fragPos3D,1.0);
     vec2 pos = fragPos3D.xz;
     vec2 ddx_pos = dFdx(pos);
     vec2 ddy_pos = dFdy(pos);
     gl_FragDepth = clip_space_pos.z/clip_space_pos.w;
    FragColor = vec4(1.0,1.0,1.0, 1.0-gridTextureGradBox(pos,ddx_pos,ddy_pos)); // opacity = 1 when t > 0, opacity = 0 otherwise

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
    GridPipeline = GPURef<VK::Pipeline>::MakeRef(VK::PipelineProperties{
        .Layout = GridPipelineLayout,
        .VariantProperties = VK::GraphicsPipelineProperties{
            .VertexBinary = VertResult.data,
            .FragmentBinary = FragResult.data,
            .VertexAttributes = {},
            .ColorAttachments = {VK::GraphicsPipelineAttachment{
                .format = Render::PresentStageAttachments.at(Render::ColorAttachmentName).format.toVKFormat().value(),
                .Blending = VK::AttachmentBlending{.BlendEnable = true,.SrcAlphaFactor = VK::BlendFactor::eZero,.DstAlphaFactor = VK::BlendFactor::eOne}}},
            .DepthAttachment =
                VK::GraphicsPipelineAttachment{
                    .format =
                        Render::PresentStageAttachments.at(Render::DepthAttachmentName).format.toVKFormat().value()},
            .cullMode = VK::CullMode::eNone,
            .DepthTest = true,
            .DepthWrite = false,

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
    CmdManager->Push(VK::CopyImageCommand(
        Context.previousLayers.front()->GetAttachment(Render::ColorAttachmentName),
        GetAttachment(Render::ColorAttachmentName), VK::ImageLayouts::eGeneral, VK::ImageLayouts::eGeneral, ivec3(0),
        ivec3(0), ivec3(Resolution, 1),
        VK::CopyImageCommand::SubresourceRange{
            .AspectMask = VK::ImageAspect::eColor, .mipLevel = 0, .baseArrayLayer = 0, .LayerCount = 1},
        VK::CopyImageCommand::SubresourceRange{
            .AspectMask = VK::ImageAspect::eColor, .mipLevel = 0, .baseArrayLayer = 0, .LayerCount = 1}));
    CmdManager->Push(VK::CopyImageCommand(
        Context.previousLayers.front()->GetAttachment(Render::DepthAttachmentName),
        GetAttachment(Render::DepthAttachmentName), VK::ImageLayouts::eGeneral, VK::ImageLayouts::eGeneral, ivec3(0),
        ivec3(0), ivec3(Resolution, 1),
        VK::CopyImageCommand::SubresourceRange{
            .AspectMask = VK::ImageAspect::eDepth, .mipLevel = 0, .baseArrayLayer = 0, .LayerCount = 1},
        VK::CopyImageCommand::SubresourceRange{
            .AspectMask = VK::ImageAspect::eDepth, .mipLevel = 0, .baseArrayLayer = 0, .LayerCount = 1}));

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

#include "GBufferRenderer.hpp"
#include "Components/CameraComponent.hpp"
#include "Components/MeshDescriptor.hpp"
#include "Components/TransformComponent.hpp"
#include "RenderingDefinitions.hpp"
#include "World.hpp"
#include <Graphics/Pipelines/ShaderCompiler.hpp>
#include <Graphics/Vulkan/Commands/CommandManager/VKBufferCommands.hpp>
#include <Graphics/Vulkan/Commands/CommandManager/VKImageCommands.hpp>
#include <Graphics/Vulkan/Commands/CommandManager/VKPipelineCommands.hpp>
#include <Graphics/Vulkan/Commands/CommandManager/VKRenderingCommands.hpp>
Graphics::CompositeLayerExecute GBufferRenderer::OnRender(const Graphics::CompositeContext &Context)
{
    const uvec2 Resolution = GetResolution();
    const float AspectRatio = (float)Resolution.x / Resolution.y;
    Owner.GetComponent<CameraComponent>().AspectRatio = AspectRatio;
    mat4 *mapped = (mat4 *)InstanceVertexBuffer->Map();

    const mat4 viewMat = Owner.GetComponent<const CameraComponent>().GetViewMatrix(),
               ProjMat = Owner.GetComponent<const CameraComponent>().GetProjMatrix();
    mat4 VP = ProjMat * viewMat;
    auto TransformMeshView = World::Get().GetComponentView<const TransformComponent, const MeshDescriptor>();
    for (auto entity : TransformMeshView)
    {
        const Transform &transform = TransformMeshView.get<const TransformComponent>(entity);
        *mapped = VP * transform.GetModelMatrix();
        mapped++;
    }
    InstanceVertexBuffer->Unmap();

    return ExecuteOutput;
}

GBufferRenderer::GBufferRenderer(std::shared_ptr<Graphics::CompositeGroup> Parent, Entity Owner)
    : ICompositeLayer(Parent, Graphics::CompositeLayerProperties{.Name = "CameraRenderer",
                                                                 .Attachments = Render::GBufferStageAttachments}),
      Owner(Owner)
{
    vkCmdMngr->PushGroupLabel("World Rendering");
    renderTarget.Make();
    renderTarget->AttachColor(0, GetAttachments().at(Render::AlbedoAttachmentName));
    renderTarget->AttachColor(1, GetAttachments().at(Render::NormalAttachmentName));
    renderTarget->AttachColor(2, GetAttachments().at(Render::PositionAttachmentName));
    renderTarget->AttachColor(3, GetAttachments().at(Render::MaterialAttachmentName));
    renderTarget->AttachDepth(GetAttachments().at(Render::DepthAttachmentName));
    renderTarget->SetViewport({0, 0}, GetResolution());

    std::pair<std::vector<std::byte>, std::vector<std::byte>> BinaryVKGL[2];
    std::ifstream testVertShaderFile("assets/shaders/VKGLtest.vert");
    std::ostringstream ssVert;
    ssVert << testVertShaderFile.rdbuf(); // copy file buffer into stringstream

    const auto VertResult = Graphics::ShaderCompiler::Get().CompileInterop(
        Graphics::InteropShaderCompileInfo{.shader_type = Graphics::ShaderType::eVertex,
                                           .source_type = Graphics::ShaderSourceType::eGLSL,
                                           .Source = ssVert.str()});
    std::ifstream testFragShaderFile("assets/shaders/VKGLtest.frag");
    std::ostringstream ssFrag;

    ssFrag << testFragShaderFile.rdbuf(); // copy file buffer into stringstream

    const auto FragResult = Graphics::ShaderCompiler::Get().CompileInterop(
        Graphics::InteropShaderCompileInfo{.shader_type = Graphics::ShaderType::eFragment,
                                           .source_type = Graphics::ShaderSourceType::eGLSL,
                                           .Source = ssFrag.str()});
    if (!FragResult.CompileSuccessful || !VertResult.CompileSuccessful)
    {
        std::cerr << "Frag:\n" << FragResult.CompileLog << "\nVert:\n" << VertResult.CompileLog;
    }
    BinaryVKGL[0].first = std::move(VertResult.VKResult.data);
    BinaryVKGL[0].second = std::move(VertResult.GLResult.data);
    BinaryVKGL[1].first = std::move(FragResult.VKResult.data);
    BinaryVKGL[1].second = std::move(FragResult.GLResult.data);
    GPURef<VK::PipelineLayout> layout = GPURef<VK::PipelineLayout>::MakeRef(VK::PipelineLayoutProperties{});
    pipeline = GPURef<VK::Pipeline>::MakeRef(VK::PipelineProperties{
        .Layout = layout,
        .VariantProperties = VK::GraphicsPipelineProperties{

            .VertexBinary = BinaryVKGL[0].first,
            .FragmentBinary = BinaryVKGL[1].first,
            .VertexAttributes = {VK::VertexAttribute{.Binding = 0,
                                                     .format = VK::Format::eRGB32_SFloat,
                                                     .Stride = sizeof(Mesh::Vertex),
                                                     .Offset = 0},
                                 VK::VertexAttribute{.Binding = 1,
                                                     .format = VK::Format::eRGBA32_SFloat,
                                                     .Stride = sizeof(mat4),
                                                     .Offset = 0,
                                                     .Rate = VK::VertexInputRate::eInstance},
                                 VK::VertexAttribute{.Binding = 2,
                                                     .format = VK::Format::eRGBA32_SFloat,
                                                     .Stride = sizeof(mat4),
                                                     .Offset = sizeof(vec4),
                                                     .Rate = VK::VertexInputRate::eInstance},
                                 VK::VertexAttribute{.Binding = 3,
                                                     .format = VK::Format::eRGBA32_SFloat,
                                                     .Stride = sizeof(mat4),
                                                     .Offset = sizeof(vec4) * 2,
                                                     .Rate = VK::VertexInputRate::eInstance},
                                 VK::VertexAttribute{.Binding = 4,
                                                     .format = VK::Format::eRGBA32_SFloat,
                                                     .Stride = sizeof(mat4),
                                                     .Offset = sizeof(vec4) * 3,
                                                     .Rate = VK::VertexInputRate::eInstance}},
            .DepthTest = true,
            .DepthWrite = true,
            .DepthAttachmentFormat = VK::Format::eDepth32_SFloat,
        }});

    auto TransformMeshView = World::Get().GetComponentView<const TransformComponent, const MeshDescriptor>();

    vkCmdMngr->BeginRendering(renderTarget);
    vkCmdMngr->Push<VK::BindPipelineCommand>(pipeline, VK::PipelineBindPoint::eGraphics);
    const size_t MeshCount = TransformMeshView.size_hint();
    InstanceVertexBuffer.Make(
        VK::BufferProperties{.Size = sizeof(mat4) * MeshCount,
                             .UsageFlags = {VK::BufferUsage::eTransferDst, VK::BufferUsage::eVertexBuffer}});

    int i = 0;
    for (auto entity : TransformMeshView)
    {
        const TransformComponent &transform = TransformMeshView.get<const TransformComponent>(entity);
        const MeshDescriptor &meshDescriptor = TransformMeshView.get<const MeshDescriptor>(entity);

        vkCmdMngr->Push(VK::BindVertexBuffersCommand(
            (size_t)0, {meshDescriptor.mesh->GetVertexBuffer(), InstanceVertexBuffer}, {(size_t)0, i * sizeof(mat4)}));
        vkCmdMngr->Push(VK::BindIndexBufferCommand(meshDescriptor.mesh->GetIndexBuffer(), 0, VK::IndexType::eUint32));
        vkCmdMngr->Push(VK::DrawIndexedCommand(meshDescriptor.mesh->IndexCount()));
        i++;
    }

    vkCmdMngr->EndRendering();
    vkCmdMngr->PopGroupLabel();
    ExecuteOutput.CommandManagers = {vkCmdMngr};
}
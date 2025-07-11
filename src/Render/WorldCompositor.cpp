#include "WorldCompositor.hpp"
#include "World.hpp"
#include <Graphics/Pipelines/ShaderCompiler.hpp>
#include <Graphics/Vulkan/Commands/CommandManager/BufferCommands.hpp>
#include <Graphics/Vulkan/Commands/CommandManager/ImageCommands.hpp>
#include <Graphics/Vulkan/Commands/CommandManager/PipelineCommands.hpp>
#include <Graphics/Vulkan/Commands/CommandManager/RenderingCommands.hpp>
WorldCompositor::WorldCompositor(std::shared_ptr<Graphics::CompositeGroup> Parent)
    : ICompositeLayer(
          Parent,
          Graphics::CompositeLayerProperties{
              .Name = "WorldCompositor",
              .Attachments = {
                  {"Color",
                   Graphics::CompositeLayerAttachmentProperties{.format = Graphics::TextureFormatValues::eRGBA8_SRGB}},
                  {"Depth",
                   Graphics::CompositeLayerAttachmentProperties{.format =
                                                                    Graphics::TextureFormatValues::eDepth32_SFloat}},
              }})
{
    vkCmdMngr->PushGroupLabel("World Rendering");
    vkCmdMngr->Push<VK::ClearColorImageCommand>(GetAttachments().at("Color"), VK::ImageLayouts::eGeneral,
                                                vec4{0, 0, 0, 0});
    vkCmdMngr->Push<VK::ClearDepthStencilImageCommand>(GetAttachments().at("Depth"), VK::ImageLayouts::eGeneral, 1.0f);
    renderTarget.Make();
    renderTarget->AttachColor(0, GetAttachments().at("Color"));
    renderTarget->AttachDepth(GetAttachments().at("Depth"));
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
                                 VK::VertexAttribute{.Binding = 1,
                                                     .format = VK::Format::eRGBA32_SFloat,
                                                     .Stride = sizeof(mat4),
                                                     .Offset = sizeof(vec4),
                                                     .Rate = VK::VertexInputRate::eInstance},
                                 VK::VertexAttribute{.Binding = 1,
                                                     .format = VK::Format::eRGBA32_SFloat,
                                                     .Stride = sizeof(mat4),
                                                     .Offset = sizeof(vec4) * 2,
                                                     .Rate = VK::VertexInputRate::eInstance},
                                 VK::VertexAttribute{.Binding = 1,
                                                     .format = VK::Format::eRGBA32_SFloat,
                                                     .Stride = sizeof(mat4),
                                                     .Offset = sizeof(vec4) * 3,
                                                     .Rate = VK::VertexInputRate::eInstance}},
            .DepthTest = true,
            .DepthWrite = true,
            .DepthAttachmentFormat = VK::Format::eDepth32_SFloat,
        }});

    auto TransformMeshView = World::Get().GetComponentView<const Transform, const MeshDescriptor>();

    vkCmdMngr->BeginRendering(renderTarget);
    vkCmdMngr->Push<VK::BindPipelineCommand>(pipeline, vk::PipelineBindPoint::eGraphics);
    const size_t MeshCount = TransformMeshView.size_hint();
    InstanceVertexBuffer.Make(
        VK::BufferProperties{.Size = sizeof(mat4) * MeshCount,
                             .UsageFlags = {VK::BufferUsage::eTransferDst, VK::BufferUsage::eVertexBuffer}});

    int i = 0;
    for (auto entity : TransformMeshView)
    {
        const Transform &transform = TransformMeshView.get<const Transform>(entity);
        const MeshDescriptor &meshDescriptor = TransformMeshView.get<const MeshDescriptor>(entity);

        vkCmdMngr->Push(VK::BindVertexBuffersCommand(
            (size_t)0, {meshDescriptor.mesh->GetVertexBuffer(), InstanceVertexBuffer}, {(size_t)0, i * sizeof(mat4)}));
        vkCmdMngr->Push(VK::BindIndexBufferCommand(meshDescriptor.mesh->GetIndexBuffer(), 0, VK::IndexType::eUint32));
        vkCmdMngr->Push(VK::DrawIndexedCommand(meshDescriptor.mesh->IndexCount()));
        i++;
    }

    vkCmdMngr->EndRendering();
    vkCmdMngr->PopGroupLabel();
}
Graphics::CompositeLayerExecute WorldCompositor::OnRender(const Graphics::CompositeContext &Context)
{
    mat4 *mapped = (mat4 *)InstanceVertexBuffer->Map();

    auto cameraView = World::Get().GetComponentView<const Camera, const Transform>();
    auto CameraEntity = *cameraView.begin();
    const mat4 viewMat = cameraView.get<const Camera>(CameraEntity)
                             .GetViewMatrix(cameraView.get<const Transform>(CameraEntity)),
               ProjMat = cameraView.get<const Camera>(CameraEntity).GetProjMatrix();
    mat4 VP = ProjMat * viewMat;
    auto TransformMeshView = World::Get().GetComponentView<const Transform, const MeshDescriptor>();
    for (auto entity : TransformMeshView)
    {
        const Transform &transform = TransformMeshView.get<const Transform>(entity);
        *mapped = VP * transform.GetModelMatrix();
        mapped++;
    }
    InstanceVertexBuffer->Unmap();

    static Graphics::CompositeLayerExecute Execute{.CommandManagers = {vkCmdMngr}};
    return Execute;
}

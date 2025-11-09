#include "EntityMaterialRenderer.hpp"
#include "Entity/World.hpp"
#include "Graphics/Pipelines/ShaderCompiler.hpp"
#include "Render/Mesh/Mesh.hpp"
bool Render::EntityMaterialRenderer::Build()
{
    std::ifstream testVertShaderFile("assets/shaders/VKGLtest.vert");
    std::ostringstream ssFile;
    ssFile << testVertShaderFile.rdbuf(); // copy file buffer into stringstream
    const auto VertCompile = Graphics::ShaderCompiler::Get().Compile(
        Graphics::ShaderCompileInfo{.shader_type = Graphics::ShaderType::eVertex,
                                    .source_type = Graphics::ShaderSourceType::eGLSL,
                                    .Source = ssFile.str()});
    std::ifstream testFragShaderFile("assets/shaders/VKGLtest.frag");
    ssFile.clear();
    ssFile << testFragShaderFile.rdbuf();
    const auto FragCompile = Graphics::ShaderCompiler::Get().Compile(
        Graphics::ShaderCompileInfo{.shader_type = Graphics::ShaderType::eFragment,
                                    .source_type = Graphics::ShaderSourceType::eGLSL,
                                    .Source = ssFile.str()});

    GPURef<VK::PipelineLayout> layout = GPURef<VK::PipelineLayout>::MakeRef(VK::PipelineLayoutProperties{});
    VK::GraphicsPipelineProperties GraphicsPipelineProperties = VK::GraphicsPipelineProperties{
        .VertexBinary = VertCompile.data,
        .FragmentBinary = FragCompile.data,
        .VertexAttributes = {VK::VertexAttribute{.Binding = 0,
                                                 .format = VK::Format::eRGB32_SFloat,
                                                 .Stride = sizeof(Mesh::Vertex),
                                                 .Offset = 0,
                                                 .Rate = VK::VertexInputRate::eVertex},
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
        .cullMode = VK::CullMode::eNone,
        .DepthTest = true,
        .DepthWrite = true,
    };

    for (const auto &colorAttachment : GetLayout().GetColorAttachments())
    {
        GraphicsPipelineProperties.ColorAttachments.push_back(
            VK::GraphicsPipelineAttachment{.format = colorAttachment.format});
    }
    if (const auto depthAttachment = GetLayout().GetDepthStencilAttachment(); depthAttachment.has_value())
    {
        GraphicsPipelineProperties.DepthAttachment.format = depthAttachment->format;
        GraphicsPipelineProperties.DepthTest = true;
    }
    pipeline = GPURef<VK::Pipeline>::MakeRef(
        VK::PipelineProperties{.Layout = layout, .VariantProperties = GraphicsPipelineProperties});
    return true;
}

void Render::EntityMaterialRenderer::Compose(Composite::Payload &payload)
{
    /*
    const mat4 viewMat = Owner.GetComponent<const CameraComponent>().GetViewMatrix(),
               ProjMat = Owner.GetComponent<const CameraComponent>().GetProjMatrix();
    mat4 VP = ProjMat * viewMat;
    auto TransformMeshView = World::Get().GetComponentView<const TransformComponent, const MeshDescriptor>();
    for (auto entity : TransformMeshView)
    {
        const TransformComponent &transform = TransformMeshView.get<const TransformComponent>(entity);
        *mapped = VP * transform.GetModelMatrix();
        mapped++;
    }
    InstanceVertexBuffer->Unmap();*/
}

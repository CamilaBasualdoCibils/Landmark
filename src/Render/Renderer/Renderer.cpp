#include "Renderer.hpp"
#include "Graphics/GraphicsEngine.hpp"
#include "Render/Compositor/Compositor.hpp"

#include "Graphics/Pipelines/ShaderCompiler.hpp"
#include "Graphics/Vulkan/Buffer/VKBuffer.hpp"
#include "Graphics/Vulkan/Commands/CommandManager/VKBufferCommands.hpp"
#include "Graphics/Vulkan/Commands/CommandManager/VKImageCommands.hpp"
#include "Graphics/Vulkan/Commands/CommandManager/VKPipelineCommands.hpp"
#include "Graphics/Vulkan/Commands/CommandManager/VKRenderingCommands.hpp"
#include "Graphics/Vulkan/Pipeline/VKPipeline.hpp"
#include "Graphics/Vulkan/VKEnums.hpp"
class SimpleLayer : public Render::Composite::Layer
{
    GPURef<VK::Buffer> VertexBuffer;
    GPURef<VK::Pipeline> pipeline;

  public:
    SimpleLayer() : Layer("Simple Layer")
    {
    }
    bool Build() override
    {
        VertexBuffer.Make(VK::BufferProperties{.Size = 3 * sizeof(vec3), .UsageFlags = VK::BufferUsage::eVertexBuffer});
        vec3 vs[3] = {{-1, -1, 0}, {1, -1, 0}, {0, 1, 0}};
        VertexBuffer->InsertData(vs, sizeof(vs), 0);
        const auto VertCompile = Graphics::ShaderCompiler::Get().Compile(
            Graphics::ShaderCompileInfo{.shader_type = Graphics::ShaderType::eVertex,
                                        .source_type = Graphics::ShaderSourceType::eGLSL,
                                        .Source = R"(
        #version 460
        layout (location = 0) in vec3 InPos;
        layout(location = 0) out vec3 Color;
        void main() {
            const vec2 pos[3] = vec2[](
                vec2(-1.0, -1.0),
                vec2( 1, -1.0),
                vec2(0,  1)
            );
            Color = vec3(pos[gl_VertexIndex],0);
            gl_Position = vec4(InPos,1);
        }
        )"});
        const auto FragCompile = Graphics::ShaderCompiler::Get().Compile(
            Graphics::ShaderCompileInfo{.shader_type = Graphics::ShaderType::eFragment,
                                        .source_type = Graphics::ShaderSourceType::eGLSL,
                                        .Source = R"(
        #version 460
        layout(location = 0) out vec4 FragColor;
        layout(location = 0) in vec3 Color;
        void main() {
        const vec2 pos[3] = vec2[](
            vec2(-1.0, -1.0),
            vec2( 1, -1.0),
            vec2(0,  1));
        FragColor = vec4(Color,1);
        }
        )"});
        GPURef<VK::PipelineLayout> layout = GPURef<VK::PipelineLayout>::MakeRef(VK::PipelineLayoutProperties{});
        VK::GraphicsPipelineProperties graphicsPipelineProeprties{};
        for (const auto ColorAttchment : GetLayout().GetColorAttachments())
        {
            graphicsPipelineProeprties.ColorAttachments.push_back(
                VK::GraphicsPipelineAttachment{.format = ColorAttchment.format});
        }
        if (const auto &DepthAttach = GetLayout().GetDepthStencilAttachment(); DepthAttach.has_value())
        {
            graphicsPipelineProeprties.DepthAttachment = VK::GraphicsPipelineAttachment{.format = DepthAttach->format};
        }
        graphicsPipelineProeprties.VertexBinary = VertCompile.data;
        graphicsPipelineProeprties.FragmentBinary = FragCompile.data;
        graphicsPipelineProeprties.VertexAttributes = {
            VK::VertexAttribute{.Binding = 0,
                                .format = VK::Format::eRGB32_SFloat,
                                .Stride = sizeof(vec3),
                                .Offset = 0,
                                .Rate = VK::VertexInputRate::eVertex},
        };
        graphicsPipelineProeprties.cullMode = VK::CullMode::eNone;
        pipeline.Make(VK::PipelineProperties{.Layout = layout, .VariantProperties = graphicsPipelineProeprties});
        return true;
    };
    void Compose(Render::Composite::Payload &Payload) override
    {
        // Payload.pipelinePayload.cmdManager->Push(VK::ClearColorImageCommand(
        //     Payload.ColorAttachments.front().Texture, VK::ImageLayouts::eGeneral, vec4(1, 0, 0, 1)));
        Payload.pipelinePayload.cmdManager->Push(VK::BindPipelineCommand(pipeline, VK::PipelineBindPoint::eGraphics));
        Payload.pipelinePayload.cmdManager->Push(VK::BindVertexBuffersCommand(0, {VertexBuffer}, {0}));
        Payload.pipelinePayload.cmdManager->Push(VK::DrawCommand(3, 1));
    }
};
Renderer::Renderer()
{
    DeferredRenderingLayout.PushColorAttachment(
        Render::Composite::ColorAttachmentInfo{.Name = "Abledo", .format = VK::Format::eRGBA8_UNorm});
    RenderPipeline = Render::Compositor::Get().MakeBasePipeline(
        "Renderer", Render::Composite::PipelineProperties{
                        .settings = Render::Composite::PipelineSettings{.BaseResolution = {1920, 1080}},
                        .layout = DeferredRenderingLayout});
    RenderPipeline->Push<SimpleLayer>();

    // mainDeferredPass =
    // Platform::WindowManager::Get().GetMainWindow()->GetPipeline()->Push<Render::DeferredRenderPass>("Main Deferred
    // Pass");
}
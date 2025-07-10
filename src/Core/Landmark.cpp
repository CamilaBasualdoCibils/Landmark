#include "Landmark.hpp"
#include "Graphics/Compositor/Compositor.hpp"
#include "Graphics/GPURef.hpp"
#include "Graphics/OpenGL/Commands/CommandManager.hpp"
#include "Graphics/OpenGL/Commands/TextureCommands.hpp"
#include "Graphics/Pipelines/ShaderCompiler.hpp"
#include "Graphics/Rendering/RenderTarget.hpp"
#include "Graphics/Synchronization.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Vulkan/Buffer/Buffer.hpp"
#include "Graphics/Vulkan/Commands/CommandManager/BufferCommands.hpp"
#include "Graphics/Vulkan/Commands/CommandManager/CommandManager.hpp"
#include "Graphics/Vulkan/Commands/CommandManager/ImageCommands.hpp"
#include "Graphics/Vulkan/Commands/CommandManager/PipelineCommands.hpp"
#include "Graphics/Vulkan/Commands/CommandManager/RenderingCommands.hpp"
#include "Graphics/Vulkan/Enums.hpp"
#include "Graphics/Vulkan/Pipeline/Pipeline.hpp"
#include "Graphics/Vulkan/Swapchain.hpp"
#include "ImGuiCompositor.hpp"
#include "WorldCompositor.hpp"
#include "pch.h"
#include <Graphics/GraphicsEngine.hpp>
#include <Graphics/Vulkan/Instance.hpp>
Landmark::Landmark(const LandmarkStartProperties &Properties)
{

    GraphicsEngine::Make(GraphicsEngineProperties{});

    std::shared_ptr<WorldCompositor> WorldComposite =
        GraphicsEngine::Get().GetMainWindow()->GetCompositeGroup()->AddLayer<WorldCompositor>();
    std::shared_ptr<ImGuiCompositor> ImGuiComposite =
        GraphicsEngine::Get().GetMainWindow()->GetCompositeGroup()->AddLayer<ImGuiCompositor>();

    GPURef<Graphics::Texture> TestTexture = GPURef<Graphics::Texture>::MakeRef(Graphics::TextureProperties{
        .Dimensions = {1024, 1024, 1}, .Levels = 1, .Format = Graphics::TextureFormatValues::eRGBA8_UNorm});

    GPURef<Graphics::RenderTarget> RenderTarget = GPURef<Graphics::RenderTarget>::MakeRef();
    RenderTarget->AttachColor(0, TestTexture);
    RenderTarget->SetViewport({0, 0}, {1024, 1024});

    GPURef<Graphics::Semaphore> ImageReadySemaphore = GPURef<Graphics::Semaphore>::MakeRef(),
                                RenderingFinishedSemaphore = GPURef<Graphics::Semaphore>::MakeRef();

    std::shared_ptr<VK::CommandManager> vulkan = std::make_shared<VK::CommandManager>(VK::CmdCapatiblities::eGeneric);
    std::shared_ptr<GL::CommandManager> openGL = std::make_shared<GL::CommandManager>();

    {
        std::shared_ptr<VK::CommandManager> PrepareCmdBuffer =
            std::make_shared<VK::CommandManager>(VK::CmdCapatiblities::eGeneric);
        for (int i = 0; i < 3; i++)
        {
            PrepareCmdBuffer->Push<VK::TransitionImageLayoutCommand>(
                GraphicsEngine::Get().GetMainWindow()->GetSwapchain()->GetImage(i).imageRaw,
                VK::ImageLayouts::eUndefined, VK::ImageLayouts::eGeneral, VK::PipelineStageFlags::eTopOfPipe,
                VK::PipelineStageFlags::eAllCommands);
        }
        auto fence = PrepareCmdBuffer->SignalFence();
        GraphicsEngine::Get().ExecuteNow({PrepareCmdBuffer});
        fence->WaitAndReset();
    }

    GPURef<VK::Pipeline> pipeline;
    {
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
                .VertexAttributes = {VK::VertexAttribute{
                    .Binding = 0, .format = VK::Format::eRG32_SFloat, .Stride = sizeof(vec2), .Offset = 0}}}});
    }
    vec2 Verts[] = {{-1, -1}, {1, -1}, {0, 1}};

    GPURef<VK::Buffer> TestVertexBuffer = GPURef<VK::Buffer>::MakeRef(VK::BufferProperties{
        .Size = sizeof(Verts),
        .UsageFlags = {VK::BufferUsage::eTransferDst, VK::BufferUsage::eTransferSrc, VK::BufferUsage::eVertexBuffer}});
    {
        void *mappedMemory = TestVertexBuffer->Map();
        std::memcpy(mappedMemory, Verts, sizeof(Verts));
        TestVertexBuffer->Unmap();
    }
    while (!GraphicsEngine::Get().GetMainWindow()->GetShouldClose())
    {
        // GraphicsEngine::Get().GetMainWindow()->GetSwapchain()->AcquireNextSwapchainImage(ImageReadySemaphore);
        GraphicsEngine::Get().BeginFrame();
        /*
        vulkan->WaitSemaphore(ImageReadySemaphore); // wait until monitor is ready

        // clear testTexture to green
        vulkan->Push<VK::ClearColorImageCommand>(TestTexture, VK::ImageLayouts::eGeneral, vec4{0, 0, 0, 1});

        openGL->WaitSemaphore(vulkan->SignalSemaphore()); // OpenGl wait for vulkan

        // Clear the top half of the texture to red
        openGL->Push<GL::ClearTextureImage>(TestTexture, vec<4, uint8_t>(0, 0, 255, 1), uvec3(0, 0, 0),
                                            uvec3(512, 256, 1));

        vulkan->WaitSemaphore(openGL->SignalSemaphore()); // Vulkan wait for OpenGL

        // Blit contents of Test Texture to monitor image
        vulkan->BeginRendering(RenderTarget);
        vulkan->Push<VK::BindPipelineCommand>(pipeline, vk::PipelineBindPoint::eGraphics);
        vulkan->Push(VK::BindVertexBuffersCommand((size_t)0, {TestVertexBuffer}, {(size_t)0}));
        vulkan->Push<VK::DrawCommand>(3, 1, 0, 0);
        vulkan->EndRendering();
        vulkan->Push<VK::ClearColorImageCommand>(
            GraphicsEngine::Get().GetMainWindow()->GetSwapchain()->GetCurrentImage().imageRaw,
            VK::ImageLayouts::eGeneral, vec4(0, 0, 0, 0));
        vulkan->Push<VK::BlitImageCommand>(
            TestTexture, GraphicsEngine::Get().GetMainWindow()->GetSwapchain()->GetCurrentImage().imageRaw,
            VK::ImageLayouts::eGeneral, VK::ImageLayouts::eGeneral, VK::Filter::eNearest, ivec3{0, 0, 0},
            ivec3{0, 0, 0}, ivec3{1024, 1024, 0});

        // met me know when youre done
        vulkan->SignalSemaphore(RenderingFinishedSemaphore);
*/
        // Submit OpenGL and Vulkan for execution
        // GraphicsEngine::Get().Push({openGL, vulkan});
        GraphicsEngine::Get().EndFrame();
        GraphicsEngine::Get().Render();
        // Present Image once Rendering has finished

        // vulkan->Clear();
        // openGL->Clear();
    }
}

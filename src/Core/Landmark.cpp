#include "Landmark.hpp"
#include "Graphics/GPURef.hpp"
#include "Graphics/ICommandBuffer.hpp"
#include "Graphics/OpenGL/CommandBuffer/CommandBuffer.hpp"
#include "Graphics/OpenGL/CommandBuffer/TextureCommands.hpp"
#include "Graphics/Pipelines/ShaderCompiler.hpp"
#include "Graphics/Rendering/RenderTarget.hpp"
#include "Graphics/Synchronization.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Vulkan/CommandBuffer/CommandBuffer.hpp"
#include "Graphics/Vulkan/CommandBuffer/ImageCommands.hpp"
#include "Graphics/Vulkan/CommandBuffer/PipelineCommands.hpp"
#include "Graphics/Vulkan/Images/ImageEnums.hpp"
#include "Graphics/Vulkan/Pipeline/PipelineEnums.hpp"
#include "Graphics/Vulkan/Swapchain.hpp"
#include "pch.h"
#include <Graphics/GraphicsEngine.hpp>
#include <Graphics/Vulkan/Instance.hpp>
#include <fstream>

Landmark::Landmark(const LandmarkStartProperties &Properties)
{

    GraphicsEngineProperties props;
    GraphicsEngine::Make(props);

    Graphics::TextureProperties TextureProps{
        .Dimensions = {512, 512, 1}, .Levels = 1, .Format = Graphics::TextureFormatValues::eRGBA8_UNorm};

    GPURef<Graphics::Texture> TestTexture = GPURef<Graphics::Texture>::MakeRef(TextureProps);
    GPURef<Graphics::RenderTarget> RenderTarget = GPURef<Graphics::RenderTarget>::MakeRef();
    RenderTarget->AttachColor(0, TestTexture);
    RenderTarget->SetViewport({0, 0}, {512, 512});

    GPURef<Graphics::Semaphore> ImageReadySemaphore = GPURef<Graphics::Semaphore>::MakeRef(),
                                RenderingFinishedSemaphore = GPURef<Graphics::Semaphore>::MakeRef();

    GPURef<VK::CommandBuffer> vulkan = GPURef<VK::CommandBuffer>::MakeRef(VK::CmdCapatiblities::eGeneric);
    GPURef<GL::CommandBuffer> openGL = GPURef<GL::CommandBuffer>::MakeRef();
    {
        GPURef<VK::CommandBuffer> TransSwapCmd = GPURef<VK::CommandBuffer>::MakeRef(VK::CmdCapatiblities::eGeneric);
        for (int i = 0; i < 3; i++)
        {
            TransSwapCmd->Push<VK::TransitionImageLayout>(
                GraphicsEngine::Get().GetMainWindow()->GetSwapchain()->GetImage(i).imageRaw,
                VK::ImageLayouts::eUndefined, VK::ImageLayouts::eGeneral, VK::PipelineStageFlags::eTopOfPipe,
                VK::PipelineStageFlags::eAllCommands);
        }
        auto fence = TransSwapCmd->SignalFence();
        GraphicsEngine::Get().ExecuteNow({TransSwapCmd});
        fence->WaitAndReset();
    }
    std::ifstream testShaderFile("assets/shaders/VKGLtest.glsl");
    std::ostringstream ss;
    ss << testShaderFile.rdbuf(); // copy file buffer into stringstream

    for (int i = 0; i < 2; i++)
    {
        Graphics::ShaderCompileInfo ShaderInfo{.shader_type = Graphics::ShaderType::eVertex,
                                               .source_type = Graphics::ShaderSourceType::eGLSL,
                                               .client_type = i == 0 ? Graphics::ShaderClientType::eOpenGL_450
                                                                     : Graphics::ShaderClientType::eVulkan_1_4,
                                               .Sources = {ss.str()}};
        const auto Result = Graphics::ShaderCompiler::Get().Compile(ShaderInfo);
        if (!Result.compile_successful)
        {
            std::cerr << Result.compile_log;
        }
    }

    while (!GraphicsEngine::Get().GetMainWindow()->GetShouldClose())
    {
        GraphicsEngine::Get().BeginFrame();
        ImGui::ShowDemoWindow();
        GraphicsEngine::Get().GetMainWindow()->GetSwapchain()->AcquireNextSwapchainImage(ImageReadySemaphore);

        vulkan->WaitSemaphore(ImageReadySemaphore); // wait until monitor is ready

        // clear testTexture to green
        vulkan->Push<VK::ClearColorImage>(TestTexture, VK::ImageLayouts::eGeneral, vec4{1, 0, 0, 1});

        openGL->WaitSemaphore(vulkan->SignalSemaphore()); // OpenGl wait for vulkan

        // Clear the top half of the texture to red
        openGL->Push<GL::ClearTextureImage>(TestTexture, vec<4, uint8_t>(0, 0, 255, 1), uvec3(0, 0, 0),
                                            uvec3(512, 256, 1));

        vulkan->WaitSemaphore(openGL->SignalSemaphore()); // Vulkan wait for OpenGL

        // Blit contents of Test Texture to monitor image
        vulkan->Push<VK::BlitImage>(TestTexture,
                                    GraphicsEngine::Get().GetMainWindow()->GetSwapchain()->GetCurrentImage().imageRaw,
                                    VK::ImageLayouts::eGeneral, VK::ImageLayouts::eGeneral, VK::Filter::eNearest,
                                    ivec3{0, 0, 0}, ivec3{0, 0, 0}, ivec3{512, 512, 0});
        vulkan->BeginRender(RenderTarget);
        vulkan->EndRender(RenderTarget);

        // met me know when youre done
        vulkan->SignalSemaphore(RenderingFinishedSemaphore);

        // Submit OpenGL and Vulkan for execution
        GraphicsEngine::Get().Push({openGL, vulkan});
        GraphicsEngine::Get().EndFrame();
        GraphicsEngine::Get().Render();
        // Present Image once Rendering has finished
        GraphicsEngine::Get().GetMainWindow()->GetSwapchain()->Present(
            GraphicsEngine::Get().GetMainGPU()->VK()->GraphicsQueue(), RenderingFinishedSemaphore);

        GraphicsEngine::Get().GetMainGPU()->VK()->LogicalDevice()->GetHandle().waitIdle();
        vulkan->Clear();
        openGL->Clear();
    }
}

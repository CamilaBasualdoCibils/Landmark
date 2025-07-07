#pragma once
#include "Graphics/ICommandBuffer.hpp"
#include "Window.hpp"
#include <Graphics/Devices/GPUInstance.hpp>
#include <Graphics/OpenGL/CommandBuffer/CommandBuffer.hpp>
#include <memory>
#include <misc/Singleton.hpp>
#include <queue>
struct GraphicsEngineProperties
{
};
class GraphicsEngine : public Singleton<GraphicsEngine, GraphicsEngineProperties>
{
    std::shared_ptr<GPUInstance> MainGPU;
    std::unordered_map<std::string, std::shared_ptr<Graphics::Window>> ActiveWindows;
    const std::string MAINWINDOWNAME = "Main";
    std::queue<GPURef<ICommandBuffer>> ExecuteCommandBuffers;

    void Dispatch(GPURef<ICommandBuffer> CmdBuffer);
  public:
    GraphicsEngine(const GraphicsEngineProperties &);

    //[[nodiscard]] std::shared_ptr<GL::CommandBuffer> MakeGLCommandBuffer();

    void Push(const std::vector<GPURef<ICommandBuffer>> &CmdBuffers);
    void ExecuteNow(const std::vector<GPURef<ICommandBuffer>> &CmdBuffers);
    void BeginFrame();
    void EndFrame();
    void Render();
    [[nodiscard]] decltype(MainGPU) GetMainGPU() const
    {
        return MainGPU;
    }

    [[nodiscard]] const decltype(ActiveWindows) &GetAllWindows() const
    {
        return ActiveWindows;
    }
    [[nodiscard]] std::shared_ptr<Graphics::Window> GetOrCreateWindow(const std::string &name)
    {
        if (!ActiveWindows.contains(name))
        {
            ActiveWindows.insert(std::make_pair(name, std::make_shared<Graphics::Window>()));
        }
        return ActiveWindows[name];
    }
    [[nodiscard]] std::shared_ptr<Graphics::Window> GetMainWindow()
    {
        return GetOrCreateWindow(MAINWINDOWNAME);
    }
    void DestroyWindow(std::shared_ptr<Graphics::Window>);
    void Update() const
    {
        glfwPollEvents();
    }
};
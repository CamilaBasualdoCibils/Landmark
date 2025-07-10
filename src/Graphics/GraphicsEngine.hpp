#pragma once
#include "Graphics/ICommandManager.hpp"
#include "Window.hpp"
#include <Graphics/Devices/GPUInstance.hpp>

#include <misc/Singleton.hpp>

struct GraphicsEngineProperties
{
};
class GraphicsEngine : public Singleton<GraphicsEngine, GraphicsEngineProperties>
{
    std::shared_ptr<GPUInstance> MainGPU;
    std::unordered_map<std::string, std::shared_ptr<Graphics::Window>> ActiveWindows;
    const std::string MAINWINDOWNAME = "Main";
    std::queue<std::shared_ptr<Graphics::ICommandManager>> ExecuteCommandBuffers;
    void Dispatch(std::shared_ptr<Graphics::ICommandManager> CmdBuffer);
  public:
    GraphicsEngine(const GraphicsEngineProperties &);

    //[[nodiscard]] std::shared_ptr<GL::CommandBuffer> MakeGLCommandBuffer();

    void Push(const std::vector<std::shared_ptr<Graphics::ICommandManager>> &CmdBuffers);
    void ExecuteNow(const std::vector<std::shared_ptr<Graphics::ICommandManager>> &CmdBuffers);
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
            ActiveWindows.insert(std::make_pair(name, std::make_shared<Graphics::Window>(name)));
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
#pragma once
#include "Graphics/ICommandManager.hpp"
#include <Graphics/Devices/GPUInstance.hpp>
#include <Graphics/Vulkan/Synchronization/VKFence.hpp>
#include <misc/Singleton.hpp>
#include <Graphics/Vulkan/Commands/CommandManager/VKCommandManager.hpp>
struct GraphicsEngineProperties
{
};
class GraphicsEngine : public Singleton<GraphicsEngine, GraphicsEngineProperties>
{
    std::shared_ptr<GPUInstance> MainGPU;
    std::queue<std::shared_ptr<Graphics::ICommandManager>> ExecuteCommandBuffers;
    std::deque<std::pair<GPURef<VK::Fence>,std::shared_ptr<VK::CommandManager>>> VkCommandBuffersInExecution;
    void Dispatch(std::shared_ptr<Graphics::ICommandManager> CmdBuffer);
  public:
    GraphicsEngine(const GraphicsEngineProperties &);

    ~GraphicsEngine()
    {
        std::cerr << "GraphicsEngine exit\n";
    }
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
    void Update() const
    {
        glfwPollEvents();
    }
};
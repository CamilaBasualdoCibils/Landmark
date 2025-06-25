#pragma once
#include <misc/Singleton.hpp>
#include <Graphics/Devices/GPUInstance.hpp>
#include <Graphics/OpenGL/CommandBuffer/CommandBuffer.hpp>
struct GraphicsEngineProperties
{

};
class GraphicsEngine : public Singleton<GraphicsEngine>
{
    std::shared_ptr<GPUInstance> MainGPU;
    public:
    GraphicsEngine(const GraphicsEngineProperties&);
    
    [[nodiscard]] std::shared_ptr<GL::CommandBuffer> MakeGLCommandBuffer();
};
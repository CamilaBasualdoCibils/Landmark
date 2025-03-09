#pragma once
#include <pch.h>
#include <Core/AppModule.h>
#include <Core/Window.h>
#include <VK/Devices/MainDevice.h>
#include <VK/Operations/CommandPool.h>
#include <VK/Operations/CommandBuffer.h>
#include <Rendering/Film.hpp>
#include <Rendering/Stage.h>
#include <Rendering/Lens.h>
#include <Rendering/Schedule.hpp>
#include <Rendering/Act.hpp>
#include <Debug/Imgui.h>

class EditorViewport;
class Renderer : public AppModule
{
    std::shared_ptr<LWindow> window;
    std::shared_ptr<MainDevice> main_device;

    std::shared_ptr<Schedule> schedule;
    std::shared_ptr<Stage> stage;
    std::shared_ptr<Act> scene_act,imgui_act;
    std::shared_ptr<Lens> lens;
    std::shared_ptr<Film> film;
    std::shared_ptr<Frame> scene_frame;
    Imgui imgui;
    EditorViewport* editor_viewport;
    public:
      std::vector<EngineCallInject> GetInjections() override;
    Renderer();

    void RenderBegin();
    void RenderEnd();
    void Init(); 

    decltype(scene_frame) GetSceneFrame() {return scene_frame;}
    

};
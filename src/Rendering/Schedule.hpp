#pragma once
#include "structs.h"
#include <VK/Operations/CommandPool.h>
#include <VK/Operations/CommandBuffer.h>
#include "Canvas.hpp"
#include "Types/Rect.hpp"
#include <Types/Viewport.h>
class Film;
class Lens;
class Act;
class Stage;
class Schedule {
    schedule_properties properties;
    std::optional<CommandPool> cmd_pool;
    std::optional<CommandBuffer> cmd_buffer;
    std::vector<Semaphore> Semaphores_before_execute;
    bool act_ongoing = false;
    std::optional<Semaphore> schedule_finished_semaphore;
    std::optional<Fence> schedule_finished_fence;

    Canvas* current_canvas;
public:
Schedule(const schedule_properties& _prop);
CommandBuffer& GetReadyCmdBuffer();
Schedule& Clear();
Schedule& Execute();
Schedule& Begin();
Schedule& End();
Schedule& BeginAct(Canvas& target,Act& act);
Schedule& EndAct();
Schedule& SetViewport(Viewport bounds);
Schedule& SetScissor(IRect2D bounds);
Schedule& PrepareNextFrame(Film& proj);
Schedule& Present(Film& proj);
Schedule& WaitUntilIdle();
Schedule& Capture(Stage& stg, const schedule_capture_stage_properties& prop);
Schedule& UseLens(Lens& lens);
Schedule& FullViewport();
Schedule& FullScissor();
Schedule& PrepareStage(Stage& stg);

};
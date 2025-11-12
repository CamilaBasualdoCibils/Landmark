//
// Created by camila on 11/10/25.
//

#ifndef LANDMARK_FRAMEGRAPHVIZ_HPP
#define LANDMARK_FRAMEGRAPHVIZ_HPP

#include "App.h"
#include "FrameGraph.hpp"
#include "Blackboard.hpp"
#include "FrameGraphResource.hpp"

class FrameGraphViz : public App {
public:
    FrameGraphViz() : App(AppProperties{
        .AppName = "FrameGraphViz", .imgui_docking_enable = true, .imgui_viewports_enable = true
    }) {
    }

private:
    void OnStart() override;

    void OnUpdate() override;

    void OnRender() override;

    void OnShutdown() override;

    struct GBufferData {
        FrameGraphResource Albedo, Normal, Depth;
    };

    Blackboard bb;
    FrameGraph fg;
};


#endif //LANDMARK_FRAMEGRAPHVIZ_HPP

//
// Created by camila on 11/10/25.
//

#include "LandmarkEngine.hpp"

#include <iostream>

#include "Graphics/Graphics.hpp"

void LandmarkEngine::Init(const Settings &settings) {
    std::cerr << "LandmarkEngine::Init()" << std::endl;
    _settings = settings;
    Graphics::Settings graphicSettings;
    graphicSettings._APIMode = Graphics::APIMode::eVulkan;
    Graphics::Get().Init(graphicSettings);
}


void LandmarkEngine::Run() {

    while (!GetShouldClose()) {
        Graphics::Get().NewFrame();

        Graphics::Get().EndFrame();
        Graphics::Get().Render();
    }
}

bool LandmarkEngine::GetShouldClose() const {
    return ShouldClose;
}

void LandmarkEngine::SetShouldClose(bool value) {
    ShouldClose = value;
}

//
// Created by camila on 11/10/25.
//

#include "Graphics.hpp"
#include "Vulkan/Instance.hpp"

#include "Libs/GLFW.hpp"
void Graphics::Init(const Settings &settings) {
  _settings = settings;
  GLFW::Settings glfwSettings;
  GLFW::Make(glfwSettings);

  InitGraphicsAPIs();
  Window::Settings windowSettings;
  _window = std::make_shared<Window>(windowSettings);
}

void Graphics::NewFrame() { glfwPollEvents(); }

void Graphics::EndFrame() {}

void Graphics::Render() { _window->SwapBuffers(); }

void Graphics::InitGraphicsAPIs() {
  Vulkan::Instance::Settings vulkan_settings;
  Vulkan::Instance::Make(vulkan_settings);
}
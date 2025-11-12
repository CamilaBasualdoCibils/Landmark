//
// Created by camila on 11/10/25.
//

#include "Graphics.hpp"


#include "Libs/GLFW.hpp"
void Graphics::Init(const Settings &settings) {

    GLFW::Settings glfwSettings;
    GLFW::Make(glfwSettings);

    Window::Settings windowSettings;
    _window = std::make_shared<Window>(windowSettings);
}

void Graphics::NewFrame() {
    glfwPollEvents();

}

void Graphics::EndFrame() {

}

void Graphics::Render() {
_window->SwapBuffers();
}

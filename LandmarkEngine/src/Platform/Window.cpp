//
// Created by camila on 11/10/25.
//

#include "Window.hpp"

Window::Window(const Settings &settings) {

    GLFW::Ensure();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, true);
    glfwWindowHint(GLFW_DECORATED, true);
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, false);
    glfwWindowHint(GLFW_SCALE_FRAMEBUFFER, false);
    _window =  glfwCreateWindow(settings.windowSize.x,settings.windowSize.y,settings.WindowTitle.c_str(), nullptr, nullptr);

    glfwMakeContextCurrent(_window);

}

bool Window::GetShouldClose() const {
    return glfwWindowShouldClose(_window);
}

void Window::SetShouldClose(bool value) {
    glfwSetWindowShouldClose(_window, value);
}

void Window::SwapBuffers() {
    glfwSwapBuffers(_window);
}

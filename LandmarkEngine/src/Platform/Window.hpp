//
// Created by camila on 11/10/25.
//

#ifndef LANDMARK_WINDOW_HPP
#define LANDMARK_WINDOW_HPP

#include "Libs/GLFW.hpp"
#include "Math/Math.hpp"
class Window {
GLFWwindow* _window;
public:
    struct Settings {
    uvec2 windowSize = {1920,1080};
        std::string WindowTitle = "Default";
    };
    Window(const Settings& settings);

    bool GetShouldClose() const;
    void SetShouldClose(bool value);
    void SwapBuffers();
};


#endif //LANDMARK_WINDOW_HPP
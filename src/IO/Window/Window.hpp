#pragma once
#include <GLFW/glfw3.h>
#include <pch.h>

class Window
{

    GLFWwindow *glfwWindowHandle;

  public:
    Window();
    bool GetShouldClose() const {return glfwWindowShouldClose(glfwWindowHandle);}
};
#pragma once
#include "misc/Singleton.hpp"
#include <GLFW/glfw3.h>
#include <IO/Window/Window.hpp>
#include <memory>
#include <pch.h>
class WindowManager : public Singleton<WindowManager>
{
    std::unordered_map<std::string, std::shared_ptr<Window>> ActiveWindows;
    const std::string MAINWINDOWNAME = "Main";
  public:
    WindowManager();

    [[nodiscard]] const decltype(ActiveWindows) &GetAllWindows() const
    {
        return ActiveWindows;
    }
    [[nodiscard]] std::shared_ptr<Window> GetOrCreateWindow(const std::string &name);
    [[nodiscard]] std::shared_ptr<Window> GetMainWindow() {return GetOrCreateWindow(MAINWINDOWNAME);}
    void DestroyWindow(std::shared_ptr<Window>);
    void Update() const
    {
        glfwPollEvents();
    }
};
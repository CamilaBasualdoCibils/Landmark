#pragma once
#include "Window.hpp"
#include "misc/Singleton.hpp"
#include "pch.h"
namespace Platform
{
class WindowManager : public Singleton<WindowManager>
{

    std::vector<std::weak_ptr<Window>> windows;
    std::shared_ptr<Window> MainWindow;

  public:
    WindowManager()
    {
        MainWindow = MakeWindow("Main");
    }
    std::shared_ptr<Window> GetMainWindow() const
    {
        LASSERT(MainWindow,"fuck?");
        return MainWindow;
    }
    std::shared_ptr<Window> MakeWindow(const std::string& name)
    {
        auto sp = std::make_shared<Window>(name);
        windows.push_back(sp);
        return sp;
    }

    void PresentAll()
    {

        for (auto it = windows.begin(); it != windows.end(); it++)
        {
            if (it->expired())
            {
                windows.erase(it);
                it--;
                continue;
                ;
            }

            it->lock()->Present();
        }
    };
};
} // namespace Platform

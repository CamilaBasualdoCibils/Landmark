#include "WindowManager.hpp"
#include "IO/Window/WindowManager.hpp"
#include <memory>
#include <utility>

WindowManager::WindowManager()
{

}
std::shared_ptr<Window> WindowManager::GetOrCreateWindow(const std::string& name)
{
    if (!ActiveWindows.contains(name))
    {
        ActiveWindows.insert(std::make_pair(name,std::make_shared<Window>()));
    }
    return ActiveWindows[name];
}
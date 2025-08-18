#pragma once
#include "EditorToolItem.hpp"
#include <pch.h>
namespace Editor
{
class ToolBar : public ToolItem
{
    std::map<int64_t, std::shared_ptr<ToolItem>> tools;
    bool IsMenuBar;

  public:
    ToolBar(const std::string &_name, bool IsMenuBar = false) : ToolItem(_name), IsMenuBar(IsMenuBar)
    {
    }

    template <typename T, typename... Args>
std::shared_ptr<T> PushObject(int64_t order = 0, Args&&... args)
{
    if (tools.find(order) != tools.end())
    {
        std::vector<int64_t> keysToShift;
        for (auto it = tools.rbegin(); it != tools.rend(); ++it)
        {
            if (it->first >= order)
                keysToShift.push_back(it->first);
            else
                break;
        }

        for (int64_t key : keysToShift)
        {
            auto nodeHandler = tools.extract(key);
            nodeHandler.key()++;
            tools.insert(std::move(nodeHandler));
        }
    }

    auto new_t = std::make_shared<T>(std::forward<Args>(args)...);
    tools.emplace(order, std::reinterpret_pointer_cast<ToolItem>(new_t));

    return new_t;
}
    template <typename T> std::shared_ptr<T> GetObject(const std::string &name)
    {

        return TryGetObject<T>(name).second;
    }
    template <typename T> std::pair<bool, std::shared_ptr<T>> TryGetObject(const std::string &name)
    {
        for (auto it = tools.begin(); it != tools.end(); ++it)
        {
            if (it->second->GetLabel() == name)
                return std::make_pair(true, std::reinterpret_pointer_cast<T>(it->second));
        }
        return std::make_pair(false, nullptr);
    }

    std::shared_ptr<ToolBar> GetOrPushToolBar(const std::string &name)
    {
        auto result = TryGetObject<ToolBar>(name);
        if (!result.first)
            return PushObject<ToolBar>(tools.empty() ? 0 : tools.rbegin()->first, name);

        return result.second;
    }
    const decltype(tools) &GetTools() const
    {
        return tools;
    }
    void DrawTool() override
    {
        for (const auto &tool : tools)
        {
            tool.second->DrawTool();
        }
    }
    void DrawHandle() override
    {

        if ((IsMenuBar && ImGui::BeginMenuBar()) || (!IsMenuBar && ImGui::BeginMenu(GetLabel().c_str())))
        {
            for (const auto &tool : tools)
            {
                tool.second->DrawHandle();
            }
            IsMenuBar ? ImGui::EndMenuBar() : ImGui::EndMenu();
        }
    }
};
} // namespace Editor

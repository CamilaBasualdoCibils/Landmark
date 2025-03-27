#pragma once

#include <pch.h>
template <typename ReturnType = void>
class EditorWindow
{
    std::string titlebar;
    bool open = true;
    using ReturnTypeValue = std::conditional_t<std::is_void<ReturnType>::value, std::monostate, std::optional<ReturnType>>;
    ReturnTypeValue return_value;

protected:
    template <typename U = ReturnType,typename std::enable_if<!std::is_void<U>::value, int>::type = 0>
    void SetReturnValue(const decltype(return_value) &v) { return_value = v; }
    virtual void DrawWindowContents() = 0;

public:
    EditorWindow(const std::string &_name) : titlebar(_name)
    {
    }
    virtual void Draw();
    template <typename U = ReturnType,typename std::enable_if<!std::is_void<U>::value, int>::type = 0>
     const decltype(return_value) &GetResult() const { return return_value; }
    constexpr bool IsOpen() const { return open; }
    constexpr void SetOpen(bool s) { open = s; }
    constexpr void SetTitleBar(const std::string &_name) { titlebar = _name; }
    constexpr const std::string &GetTitleBar() const { return titlebar; }
};

template <typename ReturnType>
inline void EditorWindow<ReturnType>::Draw()
{
    if (!open)
        return;
    if (ImGui::Begin(titlebar.c_str()))
    {
        DrawWindowContents();
        ImGui::End();
    }
}

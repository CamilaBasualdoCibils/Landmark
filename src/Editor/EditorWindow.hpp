#pragma once

#include <pch.h>
enum class EditorWindowType
{
    eWindow,
    ePopup,
    EPopupModal
};

template <typename ReturnType = void> class EditorWindow
{
    std::string titlebar;
    bool open = true;
    EditorWindowType type = EditorWindowType::eWindow;
    using ReturnTypeValue =
        std::conditional_t<std::is_void<ReturnType>::value, std::monostate, std::optional<ReturnType>>;
    ReturnTypeValue return_value;
    bool HorizontalScrollBar = false, VerticalScrollBar = true;

  protected:
    template <typename U = ReturnType, typename std::enable_if<!std::is_void<U>::value, int>::type = 0>
    void SetResult(const decltype(return_value) &v)
    {
        return_value = v;
    }
    virtual void DrawWindowContents() = 0;

  public:
    EditorWindow(const std::string &_name, EditorWindowType _type = EditorWindowType::eWindow)
        : titlebar(_name), type(_type)
    {
    }
    EditorWindow() : titlebar("Default Titlebar"), type(EditorWindowType::eWindow)
    {
    }
    virtual void Draw();
    template <typename U = ReturnType, typename std::enable_if<!std::is_void<U>::value, int>::type = 0>
    const auto &GetResult() const
    {
        return *return_value;
    }
    template <typename U = ReturnType, typename std::enable_if<!std::is_void<U>::value, int>::type = 0>
    const bool HasResult() const
    {
        return return_value.has_value();
    }
    template <typename U = ReturnType, typename std::enable_if<!std::is_void<U>::value, int>::type = 0>
    void ResetResult()
    {
        return_value.reset();
    }
    constexpr bool IsOpen() const
    {
        return open;
    }
    constexpr void SetOpen(bool s)
    {
        open = s;
    }
    constexpr void SetTitleBar(const std::string &_name)
    {
        titlebar = _name;
    }
    constexpr const std::string &GetTitleBar() const
    {
        return titlebar;
    }
    constexpr void SetWindowType(EditorWindowType _type)
    {
        type = _type;
    }
    virtual void OnWindowTryClose()
    {
        SetOpen(false);
    }
    void AllowVerticalScrollBar(bool v)
    {
        VerticalScrollBar = v;
    }
    void AllowHorizontalScrollBar(bool v)
    {
        HorizontalScrollBar = v;
    }
};

template <typename ReturnType> inline void EditorWindow<ReturnType>::Draw()
{
    if (!open)
        return;
    ImGuiWindowFlags window_flags = 0;
    //window_flags |= ImGuiWindowFlags_HorizontalScrollbar & HorizontalScrollBar;
    window_flags |= ImGuiWindowFlags_NoScrollbar & !VerticalScrollBar;
    if (type == EditorWindowType::eWindow)
    {
        bool open_state = open;
        if (ImGui::Begin(titlebar.c_str(), &open_state, window_flags))
        {
            DrawWindowContents();
        }
        ImGui::End();
        if (open_state != open)
            OnWindowTryClose();
    }
    else
    {

        ImGui::OpenPopup(titlebar.c_str(), window_flags);
        // imguipopupflags
        if (type == EditorWindowType::ePopup ? ImGui::BeginPopup(titlebar.c_str())
                                             : ImGui::BeginPopupModal(titlebar.c_str()))
        {
            DrawWindowContents();
            ImGui::EndPopup();
        }
    }
}

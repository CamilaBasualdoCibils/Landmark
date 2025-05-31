#include "MessageWindow.hpp"

MessageWindow::MessageWindow(const std::string &titlebar,EditorWindowType type, const std::string &_message, std::vector<MessageEntry> _entries)
:EditorWindow<size_t>(titlebar,type),entries(_entries)
{
    message = _message;
}

void MessageWindow::DrawWindowContents() {

    const ImVec2 avail_space = ImGui::GetContentRegionAvail();
    const int space_for_text = avail_space.y - (ImGui::CalcTextSize("aa").y + ImGui::GetStyle().FramePadding.y * 2);
    const ImVec2 message_size = ImGui::CalcTextSize(message.c_str());
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + avail_space.x/2 - message_size.x/2);
    ImGui::TextWrapped(message.c_str());
    for (int i = 0; i < entries.size();i++)
    {
        const auto& entry = entries.at(i);
        if (ImGui::Button(entry.label.c_str()))
            SetResult(i);
        if (i!= entries.size()-1)ImGui::SameLine();
        
    }
}
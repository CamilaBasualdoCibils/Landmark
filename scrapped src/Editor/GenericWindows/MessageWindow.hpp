#pragma once
#include <pch.h>
#include <Editor/EditorWindow.hpp>

class MessageWindow : public EditorWindow<size_t> {

    public:
    struct MessageEntry {
        std::string label;
    };

    MessageWindow(const std::string& titlebar,EditorWindowType type ,const std::string& _message,std::vector<MessageEntry> _entries);
    private:
    void DrawWindowContents() override;

    const std::vector<MessageEntry> entries;
    std::string message;
};
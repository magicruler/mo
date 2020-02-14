#pragma once
#include "common.h"
#include "editor_window.h"

class EditorConsole : public EditorWindow
{
    MO_OBJECT("EditorConsole")
public:
    EditorConsole(unsigned int initialWidth, unsigned int initialHeight, bool initialOpen, std::string title);
    void AddLogFormat(const char* fmt, ...);
    void AddLog(const char* msg);

    void Clear();
    virtual void OnNotify(const Event& e);

private:

    ImGuiTextBuffer     Buf;
    ImGuiTextFilter     Filter;
    ImVector<int>       LineOffsets;        // Index to lines offset. We maintain this with AddLog() calls, allowing us to have a random access on lines
    bool                AutoScroll;     // Keep scrolling if already at the bottom

    virtual void OnIMGUI();
    virtual void OnResize();
};
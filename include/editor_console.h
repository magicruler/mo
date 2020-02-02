#pragma once
#include "common.h"
#include "editor_window.h"

class EditorConsole : public EditorWindow
{
    MO_OBJECT("EditorConsole")
public:
    EditorConsole(unsigned int initialWidth, unsigned int initialHeight, bool initialOpen, std::string title);
private:
    virtual void OnIMGUI();
    virtual void OnResize();
};
#pragma once
#include "common.h"
#include "editor_window.h"

class EditorResources : public EditorWindow
{
    MO_OBJECT("EditorResources")
public:
    EditorResources(unsigned int initialWidth, unsigned int initialHeight, bool initialOpen, std::string title);
private:
    virtual void OnIMGUI();
    virtual void OnResize();
};
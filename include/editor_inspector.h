#pragma once
#include "common.h"
#include "editor_window.h"

class GameObject;

class EditorInspector : public EditorWindow
{
    MO_OBJECT("EditorInspector")
public:
    EditorInspector(unsigned int initialWidth, unsigned int initialHeight, bool initialOpen, std::string title);
private:
    virtual void OnIMGUI();
    virtual void OnResize();
};
#pragma once
#include "common.h"
#include "editor_window.h"

class Actor;

class EditorInspector : public EditorWindow
{
    MO_OBJECT("EditorInspector")
public:
    EditorInspector(unsigned int initialWidth, unsigned int initialHeight, bool initialOpen, std::string title);
    virtual void OnNotify(const Event& e);

private:
    virtual void OnIMGUI();
    virtual void OnResize();
    Actor* previousActor = nullptr;
};
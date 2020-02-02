#pragma once
#include "common.h"
#include "editor_window.h"

class RenderTarget;

class EditorGame : public EditorWindow
{
    MO_OBJECT("EditorGame")
public:
    EditorGame(unsigned int initialWidth, unsigned int initialHeight, bool initialOpen, std::string title);
private:
    virtual void OnIMGUI();
    virtual void OnResize();

    RenderTarget* gameViewRenderTarget;
    bool initialized = false;
};
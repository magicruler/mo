#pragma once
#include "common.h"
#include "imgui.h"

class EditorWindow : public Object
{
    MO_OBJECT("EditorWindow")
public:
    EditorWindow(unsigned int initialWidth, unsigned int initialHeight, bool initialOpen, std::string title);

    ~EditorWindow();

    void OnFrame();

protected:
    glm::vec2 contentMin;
    glm::vec2 contentMax;
    glm::vec2 contentSize;

private:
    virtual void Init();

    virtual void OnIMGUI();

    virtual void OnResize();

    virtual void OnOpen();

    virtual void OnClose();

private:
    std::string title;

    unsigned int initialWidth;
    unsigned int initialHeight;

    bool firstFrame = true;
    bool previousOpen = true;
    bool open = true;
};
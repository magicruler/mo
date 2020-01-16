#pragma once
#include "common.h"
#include "editor_window.h"

class GameObject;

class EditorHierarchy : public EditorWindow
{
    MO_OBJECT("EditorHierarchy")
public:
    EditorHierarchy(unsigned int initialWidth, unsigned int initialHeight, bool initialOpen, std::string title);
private:
    virtual void OnIMGUI();
    virtual void OnResize();
    
    void WindowMenu();
    void TreeView();

    std::vector<GameObject*> selection;
};
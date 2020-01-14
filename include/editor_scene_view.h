#pragma once
#include "common.h"

#include "editor_window.h"

class RenderTarget;

class EditorSceneView : public EditorWindow
{
    MO_OBJECT("EditorSceneView")
public:
    EditorSceneView(unsigned int initialWidth, unsigned int initialHeight, bool initialOpen, std::string title);

    void SetSceneViewRenderTarget(std::shared_ptr<RenderTarget> renderTarget);

private:
    virtual void OnIMGUI();

    virtual void OnResize();

    std::shared_ptr<RenderTarget> sceneViewRenderTarget;
};
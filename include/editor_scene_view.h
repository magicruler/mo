#pragma once
#include "common.h"
#include "editor_window.h"

class RenderTarget;
class Camera;
class Actor;

class EditorSceneView : public EditorWindow
{
    MO_OBJECT("EditorSceneView")
public:
    
    EditorSceneView(unsigned int initialWidth, unsigned int initialHeight, bool initialOpen, std::string title);

    //void SetSceneViewRenderTarget(RenderTarget* renderTarget);

private:
    virtual void OnIMGUI();

    void OnSceneCameraControl();
;
    virtual void OnResize();

    Camera* sceneCamera = nullptr;

    RenderTarget* sceneViewRenderTarget;
    bool initialized = false;
};
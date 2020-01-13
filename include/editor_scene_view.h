#pragma once
#include "editor_window.h"
#include "render_target.h"

class EditorSceneView : public EditorWindow
{
    MO_OBJECT("EditorSceneView")
public:
    EditorSceneView(unsigned int initialWidth, unsigned int initialHeight, bool initialOpen, std::string title) : EditorWindow(initialWidth, initialHeight, initialOpen, title)
    {
    }

    void SetSceneViewRenderTarget(std::shared_ptr<RenderTarget> renderTarget)
    {
        sceneViewRenderTarget = renderTarget;
    }

private:
    void OnIMGUI() override
    {
        ImGui::Image((ImTextureID)sceneViewRenderTarget->GetAttachmentTexture(0)->AsID(), sceneViewRenderTarget->GetSize());
    }

    void OnResize() override
    {
        sceneViewRenderTarget->Resize(contentSize.x, contentSize.y);
    }

    std::shared_ptr<RenderTarget> sceneViewRenderTarget;
};
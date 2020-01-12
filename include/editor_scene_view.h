#pragma once
#include "editor_window.h"
#include "render_target.h"

class EditorSceneView : public EditorWindow
{
public:
    EditorSceneView(unsigned int initialWidth, unsigned int initialHeight, bool initialOpen, std::string title) : EditorWindow(initialWidth, initialHeight, initialOpen, title)
    {
    }

    void SetSceneViewRenderTarget(std::shared_ptr<RenderTarget> renderTarget){
        sceneViewRenderTarget = renderTarget;
    }
    
private:
    void OnIMGUI() override
    {
        ImGui::Image((ImTextureID)sceneViewRenderTarget->GetAttachmentTexture(0)->AsID(), ImVec2(300, 300));
    }

    std::shared_ptr<RenderTarget> sceneViewRenderTarget;
};
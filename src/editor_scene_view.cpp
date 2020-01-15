#include "editor_scene_view.h"

#include "render_target.h"
#include "texture.h"

EditorSceneView::EditorSceneView(unsigned int initialWidth, unsigned int initialHeight, bool initialOpen, std::string title) : EditorWindow(initialWidth, initialHeight, initialOpen, title)
{
}

void EditorSceneView::SetSceneViewRenderTarget(RenderTarget* renderTarget)
{
    sceneViewRenderTarget = renderTarget;
}

void EditorSceneView::OnIMGUI() 
{
    auto textureId = sceneViewRenderTarget->GetAttachmentTexture(0)->AsID();
    ImGui::Image((ImTextureID)textureId, sceneViewRenderTarget->GetSize());
}

void EditorSceneView::OnResize() 
{
    sceneViewRenderTarget->Resize(contentSize.x, contentSize.y);
}
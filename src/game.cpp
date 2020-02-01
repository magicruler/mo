#include "game.h"
#include "scene.h"

#include "editor_window_system.h"
#include "render_target.h"

#include "editor_scene_view.h"
#include "camera.h"

namespace Game
{
    Scene* activeScene = nullptr;
    RenderTarget* mainRenderTarget = nullptr;
    glm::vec4 clearColor = glm::vec4(0.45f, 0.55f, 0.60f, 1.00f);

    void InitDefaultScene()
    {
        activeScene = new Scene();

        auto camera = new Camera();
        activeScene->rootNode->AddChild(camera);
    }

    void Init()
    {
        InitDefaultScene();

        mainRenderTarget = new RenderTarget(300, 300);

        EditorWindowSystem::Init();

        auto editorSceneView = EditorWindowSystem::GetInstance()->GetEditor<EditorSceneView>();
        editorSceneView->SetSceneViewRenderTarget(mainRenderTarget);
    }

    glm::vec2 MainRenderTargetGetSize()
    {
        return mainRenderTarget->GetSize();
    }

    void Update()
    {
        activeScene->Tick();
        EditorWindowSystem::GetInstance()->Update();
    }

    void Render()
    {
        mainRenderTarget->Bind();
        glm::vec2 renderTargetSize = mainRenderTarget->GetSize();
        glViewport(0, 0, (int)renderTargetSize.x, (int)renderTargetSize.y);
        glClearColor(1.0f, clearColor.y, clearColor.z, clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



        mainRenderTarget->Unbind();
    }

    void End()
    {
        EditorWindowSystem::Destroy();

        delete activeScene;
        delete mainRenderTarget;
    }
} // namespace Game
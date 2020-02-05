#include "game.h"
#include "scene.h"

#include "editor_window_system.h"
#include "render_target.h"

#include "editor_scene_view.h"
#include "camera.h"
#include "serialization.h"
#include "string_utils.h"
#include "configs.h"
#include "component_manager.h"

namespace Game
{
    Scene* activeScene = nullptr;
    RenderTarget* mainRenderTarget = nullptr;
    glm::vec4 clearColor = glm::vec4(0.45f, 0.55f, 0.60f, 1.00f);

    void InitEntryScene()
    {
        auto scenePath = "scenes/" + Configuration::GetEntryScene();
        auto sceneContent = StringUtils::ReadFile(scenePath);
        activeScene = Serialization::DeserializeScene(sceneContent);
    }

    void Init()
    {
        ComponentManager::Init();

        Serialization::LoadProject();
        mainRenderTarget = new RenderTarget(300, 300);
        InitEntryScene();

        EditorWindowSystem::Init();
    }
   
    void Update()
    {
        activeScene->Tick();
        EditorWindowSystem::GetInstance()->Update();
    }

    void Render()
    {
        auto cameras = activeScene->GetCameras();
        for (auto camera:cameras)
        {
            camera->Render();
        }
        //// for loop camera render
        //mainRenderTarget->Bind();
        //glm::vec2 renderTargetSize = mainRenderTarget->GetSize();
        //glViewport(0, 0, (int)renderTargetSize.x, (int)renderTargetSize.y);
        //glClearColor(1.0f, clearColor.y, clearColor.z, clearColor.w);
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //mainRenderTarget->Unbind();
    }

    void End()
    {
        EditorWindowSystem::Destroy();

        if (activeScene != nullptr)
        {
            delete activeScene;
        }

        delete mainRenderTarget;
    }

    /*
   API
   */
    RenderTarget* MainRenderTargetGetPointer()
    {
        return mainRenderTarget;
    }

    glm::vec2 MainRenderTargetGetSize()
    {
        return mainRenderTarget->GetSize();
    }

    Scene* ActiveSceneGetPointer()
    {
        return activeScene;
    }
} // namespace Game
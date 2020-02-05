#include "editor_scene_view.h"

#include "render_target.h"
#include "texture.h"
#include "camera.h"
#include "game.h"
#include "scene.h"
#include "time_manager.h"
#include "ray_cast.h"
#include "editor_window_system.h"
#include "ImGuizmo.h"
#include "input.h"
#include "component_manager.h"
#include "actor.h"

constexpr float CAMERA_ROTATE_SPEED = 1.5f;
constexpr float CAMERA_FORWARD_SPEED = 5.0f;
constexpr float CAMERA_XY_PLANE_SPEED = 1.0f;

static ImGuizmo::OPERATION operation = ImGuizmo::OPERATION::TRANSLATE;
static Camera* cameraCom = nullptr;

EditorSceneView::EditorSceneView(unsigned int initialWidth, unsigned int initialHeight, bool initialOpen, std::string title) : EditorWindow(initialWidth, initialHeight, initialOpen, title)
{
    sceneViewRenderTarget = new RenderTarget(initialWidth, initialHeight);
    
    sceneCamera = new Actor();

    cameraCom = ComponentManager::GetInstance()->CreateCameraComponent();
    cameraCom->renderTarget = sceneViewRenderTarget;

    sceneCamera->AddComponent(cameraCom);

    sceneCamera->SetLocalPosition(glm::vec3(0.0f, 0.0f, 12.0f));
    
    sceneCamera->SetLayerFlag(LAYER_MASK::ONLY_FOR_EDITOR_OBJECTS);
    sceneCamera->SetPropertyFlag(PROPERTY_MASK::NON_SERIALIZED);

    Game::ActiveSceneGetPointer()->rootNode->AddChild(sceneCamera);
}

glm::vec2 WorldToScreen(glm::mat4& projection, glm::mat4& view, glm::vec2 screenSize, glm::vec3 position)
{
    auto clipSpacePos = projection * (view * glm::vec4(position, 1.0f));
    auto ndcSpacePos = glm::vec3(clipSpacePos.x, clipSpacePos.y, clipSpacePos.z) / clipSpacePos.w;

    glm::vec2 windowSpacePos = ((glm::vec2(ndcSpacePos.x, -ndcSpacePos.y) + 1.0f) / 2.0f) * glm::vec2(screenSize.x, screenSize.y);
    return windowSpacePos;
}

void EditorSceneView::OnIMGUI() 
{
    OnSceneCameraControl();

    // Object Picking
    auto io = ImGui::GetIO();
    auto drawList = ImGui::GetWindowDrawList();
    const glm::vec2 p = ImGui::GetCursorScreenPos();

    if (!initialized)
    {
        initialized = true;
        sceneViewRenderTarget->Resize(contentSize.x, contentSize.y);
    }
    else
    {
        auto textureId = sceneViewRenderTarget->GetAttachmentTexture(0)->AsID();
        ImGui::Image((ImTextureID)textureId, sceneViewRenderTarget->GetSize(), ImVec2(0, 1), ImVec2(1, 0));
    }

    //// Draw AABB
    //{
        auto projection = cameraCom->GetProjection();
        auto view = cameraCom->GetViewMatrix();

    //    auto renderables = Game::ActiveSceneGetPointer()->GetRenderables();
    //    for (auto renderable : renderables)
    //    {
    //        auto aabb = renderable->GetAABB();
    //        // Face 1
    //        drawList->AddLine(p + WorldToScreen(projection, view, contentSize, glm::vec3(aabb[0].x, aabb[0].y, aabb[0].z)),
    //            p + WorldToScreen(projection, view, contentSize, glm::vec3(aabb[1].x, aabb[0].y, aabb[0].z)),
    //            ImColor(1.0f, 0.0f, 0.0f));
    //        drawList->AddLine(p + WorldToScreen(projection, view, contentSize, glm::vec3(aabb[1].x, aabb[0].y, aabb[0].z)),
    //            p + WorldToScreen(projection, view, contentSize, glm::vec3(aabb[1].x, aabb[0].y, aabb[1].z)),
    //            ImColor(1.0f, 0.0f, 0.0f));
    //        drawList->AddLine(p + WorldToScreen(projection, view, contentSize, glm::vec3(aabb[1].x, aabb[0].y, aabb[1].z)),
    //            p + WorldToScreen(projection, view, contentSize, glm::vec3(aabb[0].x, aabb[0].y, aabb[1].z)),
    //            ImColor(1.0f, 0.0f, 0.0f));
    //        drawList->AddLine(p + WorldToScreen(projection, view, contentSize, glm::vec3(aabb[0].x, aabb[0].y, aabb[1].z)),
    //            p + WorldToScreen(projection, view, contentSize, glm::vec3(aabb[0].x, aabb[0].y, aabb[0].z)),
    //            ImColor(1.0f, 0.0f, 0.0f));
    //        // Face 2
    //        drawList->AddLine(p + WorldToScreen(projection, view, contentSize, glm::vec3(aabb[0].x, aabb[1].y, aabb[0].z)),
    //                          p + WorldToScreen(projection, view, contentSize, glm::vec3(aabb[1].x, aabb[1].y, aabb[0].z)),
    //                          ImColor(1.0f, 0.0f, 0.0f));
    //        drawList->AddLine(p + WorldToScreen(projection, view, contentSize, glm::vec3(aabb[1].x, aabb[1].y, aabb[0].z)),
    //                          p + WorldToScreen(projection, view, contentSize, glm::vec3(aabb[1].x, aabb[1].y, aabb[1].z)),
    //                          ImColor(1.0f, 0.0f, 0.0f));
    //        drawList->AddLine(p + WorldToScreen(projection, view, contentSize, glm::vec3(aabb[1].x, aabb[1].y, aabb[1].z)),
    //                          p + WorldToScreen(projection, view, contentSize, glm::vec3(aabb[0].x, aabb[1].y, aabb[1].z)),
    //                          ImColor(1.0f, 0.0f, 0.0f));
    //        drawList->AddLine(p + WorldToScreen(projection, view, contentSize, glm::vec3(aabb[0].x, aabb[1].y, aabb[1].z)),
    //                          p + WorldToScreen(projection, view, contentSize, glm::vec3(aabb[0].x, aabb[1].y, aabb[0].z)),
    //                          ImColor(1.0f, 0.0f, 0.0f));

    //        drawList->AddLine(
    //            p + WorldToScreen(projection, view, contentSize, glm::vec3(aabb[0].x, aabb[0].y, aabb[0].z)),
    //            p + WorldToScreen(projection, view, contentSize, glm::vec3(aabb[0].x, aabb[1].y, aabb[0].z)),
    //            ImColor(1.0f, 0.0f, 0.0f));
    //        drawList->AddLine(
    //            p + WorldToScreen(projection, view, contentSize, glm::vec3(aabb[0].x, aabb[1].y, aabb[1].z)),
    //            p + WorldToScreen(projection, view, contentSize, glm::vec3(aabb[0].x, aabb[0].y, aabb[1].z)),
    //            ImColor(1.0f, 0.0f, 0.0f));

    //        drawList->AddLine(
    //            p + WorldToScreen(projection, view, contentSize, glm::vec3(aabb[1].x, aabb[0].y, aabb[0].z)),
    //            p + WorldToScreen(projection, view, contentSize, glm::vec3(aabb[1].x, aabb[1].y, aabb[0].z)),
    //            ImColor(1.0f, 0.0f, 0.0f));
    //        drawList->AddLine(
    //            p + WorldToScreen(projection, view, contentSize, glm::vec3(aabb[1].x, aabb[1].y, aabb[1].z)),
    //            p + WorldToScreen(projection, view, contentSize, glm::vec3(aabb[1].x, aabb[0].y, aabb[1].z)),
    //            ImColor(1.0f, 0.0f, 0.0f));

    //    }

        auto windowPos = glm::vec2(ImGui::GetMousePos()) - contentMin;
   
        if (ImGui::IsWindowHovered())
        {
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGuizmo::IsOver())
            {
                // another
                auto cameraRay = cameraCom->ScreenRay(windowPos.x, windowPos.y);

                RayCastInteraction interaction;
                Physics::RayCast(cameraRay, LAYER_MASK::GENERAL, interaction);

                if (interaction.target != nullptr)
                {
                    EditorWindowSystem::GetInstance()->SetActorSelection(interaction.target);
                    spdlog::info("Actor {} Casted By Ray", interaction.target->GetName());
                }
                else
                {
                    EditorWindowSystem::GetInstance()->ClearActorSelection();
                }
            }
        }
    // }

        auto selection = EditorWindowSystem::GetInstance()->GetActorSelection();
        if (selection.size() == 1)
        {
            auto actor = selection[0];
            
            if (Input::CheckKey(KEYBOARD_KEY::W))
            {
                operation = ImGuizmo::OPERATION::TRANSLATE;
            }
            else if (Input::CheckKey(KEYBOARD_KEY::E))
            {
                operation = ImGuizmo::OPERATION::ROTATE;
            }
            else if (Input::CheckKey(KEYBOARD_KEY::R))
            {
                operation = ImGuizmo::OPERATION::SCALE;
            }
            
            ImGuizmo::SetDrawlist();
            ImGuizmo::SetRect(contentMin.x, contentMin.y, contentSize.x, contentSize.y);
            ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(projection), operation, ImGuizmo::MODE::WORLD, glm::value_ptr(actor->transform));
            actor->UpdateLocalSpace();
        }
}

void EditorSceneView::OnResize() 
{
    spdlog::info("Scene Render Target Is {}", sceneViewRenderTarget->GetGPUHandle());
    sceneViewRenderTarget->Resize(contentSize.x, contentSize.y);
}

void EditorSceneView::OnSceneCameraControl()
{
    auto io = ImGui::GetIO();
    float deltaTime = Time::GetDeltaTime();

    if (ImGui::IsWindowHovered())
    {   
        float wheelValue = io.MouseWheel;
        if (wheelValue != 0.0f)
        {
            sceneCamera->Translate(wheelValue * CAMERA_FORWARD_SPEED * deltaTime * glm::vec3(0.0f, 0.0f, -1.0f));
        }

        if (ImGui::IsMouseDragging(ImGuiMouseButton_Middle))
        {
            glm::vec2 delta = io.MouseDelta;
            sceneCamera->Translate(deltaTime * glm::vec3(-CAMERA_XY_PLANE_SPEED * delta.x, CAMERA_XY_PLANE_SPEED * delta.y, 0.0f));
        }

        if (ImGui::IsMouseDragging(ImGuiMouseButton_Right))
        {
            glm::vec2 delta = io.MouseDelta;
            sceneCamera->Rotate(deltaTime * glm::vec3(CAMERA_ROTATE_SPEED * 0.0174f * delta.y, CAMERA_ROTATE_SPEED * 0.0174f * delta.x, 0.0f));
        }
    }
}
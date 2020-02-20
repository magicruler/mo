#include "editor_window_system.h"
#include "editor_window.h"
#include "editor_scene_view.h"
#include "editor_hierarchy.h"
#include "editor_inspector.h"
#include "editor_game.h"
#include "editor_console.h"
#include "editor_resources.h"
#include "imgui.h"
#include "actor.h"

EditorWindowSystem *EditorWindowSystem::instance = nullptr;

void EditorWindowSystem::Init()
{
    instance = new EditorWindowSystem();
    instance->InitWindows();
}

void EditorWindowSystem::Destroy()
{
    delete instance;
}

EditorWindowSystem *EditorWindowSystem::GetInstance()
{
    assert(instance != nullptr);
    return instance;
}

EditorWindow *EditorWindowSystem::GetEditorById(size_t hashId)
{
    for (size_t i = 0; i < editorWindows.size(); i++)
    {
        EditorWindow *editor = editorWindows[i];
        spdlog::info("Editor Name Is {}", editor->GetType());
        if (hashId == editor->GetHashID())
        {
            return editor;
        }
    }

    return nullptr;
}

void EditorWindowSystem::Update()
{
    static bool opt_fullscreen_persistant = true;
    bool opt_fullscreen = opt_fullscreen_persistant;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background 
    // and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    static bool p_open = true;
    ImGui::Begin("DockSpace Demo", &p_open, window_flags);
    ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // DockSpace
    ImGuiIO& io = ImGui::GetIO();

    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Save"))
            {
                
            }
        
            ImGui::EndMenu();
        }
        else  if (ImGui::BeginMenu("View"))
        {
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    ImGui::End();

    for (size_t i = 0; i < editorWindows.size(); i++)
    {
        EditorWindow *editor = editorWindows[i];
        editor->OnFrame();
    }
}

EditorWindowSystem::EditorWindowSystem()
{
}

EditorWindowSystem::~EditorWindowSystem()
{
    for(auto each : editorWindows)
    {
        delete each;
    }
}

void EditorWindowSystem::InitWindows()
{
    editorWindows.push_back(new EditorConsole(400, 200, true, "Console"));
    //editorWindows.push_back(new EditorGame(200, 200, true, "Game"));

    editorWindows.push_back(new EditorResources(800, 600, true, "Resources"));
    editorWindows.push_back(new EditorSceneView(200, 200, true, "Scene View"));

    editorWindows.push_back(new EditorHierarchy(200, 400, true, "Hierarchy"));
    editorWindows.push_back(new EditorInspector(200, 400, true, "Inspector"));
}

void EditorWindowSystem::SetActorSelection(Actor* actor)
{
    actorSelection.clear();
    actorSelection.push_back(actor);
}

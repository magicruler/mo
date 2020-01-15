#include "editor_window_system.h"
#include "editor_window.h"
#include "editor_scene_view.h"

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
    editorWindows.push_back(new EditorWindow(800, 600, true, "Base Editor Window"));
    editorWindows.push_back(new EditorWindow(200, 200, true, "Other Base Editor Window"));
    editorWindows.push_back(new EditorSceneView(200, 200, true, "Scene View"));
}

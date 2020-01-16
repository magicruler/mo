#include "game.h"

#include "editor_window_system.h"
#include "gameobject_system.h"
#include "component_system.h"
#include "scene_system.h"
#include "resource_system.h"

#include "test_object.h"
#include "event.h"

#include "task_system.h"

namespace Game
{
void Init()
{
    TaskSystem::Init();
    GameObjectSystem::Init();
    ComponentSystem::Init();
    ResourceSystem::Init();
    EditorWindowSystem::Init();
    SceneSystem::Init();
}

void Update(float dt)
{
    GameObjectSystem::GetInstance()->Update();
    ComponentSystem::GetInstance()->Update();
    EditorWindowSystem::GetInstance()->Update();
}

void End()
{
    TaskSystem::Destroy();
    GameObjectSystem::Destroy();
    ComponentSystem::Destroy();
    ResourceSystem::Destroy();
    EditorWindowSystem::Destroy();
    SceneSystem::Destroy();
}
} // namespace Game
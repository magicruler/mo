#include "game.h"

#include "editor_window_system.h"
#include "gameobject_system.h"
#include "component_system.h"
#include "scene_system.h"
#include "resource_system.h"

#include "test_object.h"
#include "event.h"

namespace Game
{
void Init()
{
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
} // namespace Game
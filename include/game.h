#pragma once

#include "editor_window_system.h"
#include "gameobject_system.h"
#include "component_system.h"
#include "scene_system.h"
#include "resource_system.h"

void InitSystem()
{
    ResourceSystem::Init();
    GameObjectSystem::Init();
    ComponentSystem::Init();
    EditorWindowSystem::Init();
    SceneSystem::Init();
}
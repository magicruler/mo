#include "scene_system.h"
#include "scene.h"

SceneSystem *SceneSystem::instance = nullptr;

SceneSystem::SceneSystem()
{
}

SceneSystem::~SceneSystem()
{
}

void SceneSystem::Init()
{
    instance = new SceneSystem();
    instance->LoadDefaultScene();
}

void SceneSystem::Destroy()
{
    delete instance;
}

SceneSystem *SceneSystem::GetInstance()
{
    assert(instance != nullptr);
    return instance;
}

Scene *SceneSystem::GetActivatedScene() const
{
    return activatedScene;
}

void SceneSystem::LoadDefaultScene()
{
    activatedScene = new Scene();
}

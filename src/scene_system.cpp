#include "scene_system.h"
#include "scene.h"

SceneSystem *SceneSystem::instance = nullptr;

SceneSystem::SceneSystem()
{
}

SceneSystem::~SceneSystem()
{
    delete instance;
}

void SceneSystem::Init()
{
    instance = new SceneSystem();
    instance->LoadDefaultScene();
}

SceneSystem *SceneSystem::GetInstance()
{
    assert(instance != nullptr);
    return instance;
}

std::shared_ptr<Scene> SceneSystem::GetActivatedScene() const
{
    return activatedScene;
}

void SceneSystem::LoadDefaultScene()
{
    activatedScene = std::make_shared<Scene>();
}

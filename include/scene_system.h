#pragma once
#include "common.h"
#include "scene.h"

class SceneSystem
{
public:
    static void Init()
    {
        instance = new SceneSystem();
        instance->LoadDefaultScene();
    }

    static SceneSystem *GetInstance()
    {
        assert(instance != nullptr);
        return instance;
    }

    std::shared_ptr<Scene> GetActivatedScene() const
    {
        return activatedScene;
    }

private:

    void LoadDefaultScene()
    {
        activatedScene = std::make_shared<Scene>();
    }

    SceneSystem()
    {
    }

    ~SceneSystem()
    {
        delete instance;
    }

    std::shared_ptr<Scene> activatedScene;

    static SceneSystem *instance;
};

SceneSystem *SceneSystem::instance = nullptr;
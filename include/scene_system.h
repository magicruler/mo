#pragma once
#include "common.h"

class Scene;

class SceneSystem
{
public:
    static void Init();

    static SceneSystem *GetInstance();

    std::shared_ptr<Scene> GetActivatedScene() const;

private:
    void LoadDefaultScene();

    SceneSystem();

    ~SceneSystem();

    std::shared_ptr<Scene> activatedScene;

    static SceneSystem *instance;
};

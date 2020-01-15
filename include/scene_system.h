#pragma once
#include "common.h"

class Scene;

class SceneSystem
{
public:
    static void Init();
    static void Destroy();

    static SceneSystem *GetInstance();

    Scene* GetActivatedScene() const;

private:
    void LoadDefaultScene();

    SceneSystem();

    ~SceneSystem();

    Scene* activatedScene;

    static SceneSystem *instance;
};

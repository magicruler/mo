#pragma once
#include "common.h"

class GameObject;

class GameObjectSystem
{
public:
    static void Init();
    static void Destroy();

    static GameObjectSystem *GetInstance();

    void AddGameObject(GameObject* gameObject);

    void Update();

private:
    GameObjectSystem();

    ~GameObjectSystem();

    static GameObjectSystem *instance;
    std::vector<GameObject*> gameObjects;
};

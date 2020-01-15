#include "gameobject_system.h"
#include "gameobject.h"

GameObjectSystem *GameObjectSystem::instance = nullptr;

GameObjectSystem::GameObjectSystem()
{
}

GameObjectSystem::~GameObjectSystem()
{
    delete instance;
}

void GameObjectSystem::Init()
{
    instance = new GameObjectSystem();
}

GameObjectSystem *GameObjectSystem::GetInstance()
{
    assert(instance != nullptr);
    return instance;
}

void GameObjectSystem::AddGameObject(GameObject* gameObject)
{
    gameObjects.push_back(gameObject);
}

void GameObjectSystem::Update()
{
    // check gameoject need destroy state
    for (int i = gameObjects.size() - 1; i >= 0; i--)
    {
        GameObject* gameObject = gameObjects[i];
        if (gameObject->needDestroy)
        {
            gameObjects.erase(gameObjects.begin() + i);
            gameObject->Clear();
            delete gameObject;
        }
    }
}

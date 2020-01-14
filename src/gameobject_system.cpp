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

void GameObjectSystem::AddGameObject(std::shared_ptr<GameObject> gameObject)
{
    gameobjects.push_back(gameObject);
}

void GameObjectSystem::Update()
{
    // check gameoject need destroy state
    for (int i = gameobjects.size() - 1; i >= 0; i--)
    {
        std::shared_ptr<GameObject> gameobect = gameobjects[i];
        if (gameobect->needDestroy)
        {
            gameobect->Clear();
            gameobjects.erase(gameobjects.begin() + i);
        }
    }
}

#pragma once
#include "common.h"
#include "gameobject.h"

class GameObjectSystem
{
public:
    static void Init()
    {
        instance = new GameObjectSystem();
    }

    static GameObjectSystem *GetInstance()
    {
        assert(instance != nullptr);
        return instance;
    }

    void AddGameObject(std::shared_ptr<GameObject> gameObject)
    {
        gameobjects.push_back(gameObject);
    }

    void Update()
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

private:
    GameObjectSystem()
    {
    }

    ~GameObjectSystem()
    {
        delete instance;
    }

    static GameObjectSystem *instance;
    std::vector<std::shared_ptr<GameObject>> gameobjects;
};

GameObjectSystem *GameObjectSystem::instance = nullptr;
#pragma once
#include "common.h"
#include "gameobject.h"

class Scene
{
public:
    void AddGameObject(std::shared_ptr<GameObject> gameObject)
    {
        root.push_back(gameObject);
    }

    void RemoveGameObject(std::shared_ptr<GameObject> gameObject)
    {
        for (size_t i = root.size(); i >= 0; i--)
        {
            if (root[i] == gameObject)
            {
                root.erase(root.begin() + i);
            }
        }
    }

private:
    std::vector<std::shared_ptr<GameObject>> root;
};
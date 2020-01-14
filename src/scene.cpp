#include "scene.h"
#include "gameobject.h"

void Scene::AddGameObject(std::shared_ptr<GameObject> gameObject)
{
    root.push_back(gameObject);
}

void Scene::RemoveGameObject(std::shared_ptr<GameObject> gameObject)
{
    for (size_t i = root.size(); i >= 0; i--)
    {
        if (root[i] == gameObject)
        {
            root.erase(root.begin() + i);
        }
    }
}
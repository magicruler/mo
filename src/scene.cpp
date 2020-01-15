#include "scene.h"
#include "gameobject.h"

void Scene::AddGameObject(GameObject* gameObject)
{
    gameObject->SetIsSceneRoot(true);
    root.push_back(gameObject);
}

void Scene::RemoveGameObject(GameObject* gameObject)
{
    for (size_t i = root.size(); i >= 0; i--)
    {
        if (root[i] == gameObject)
        {
            gameObject->SetIsSceneRoot(false);
            root.erase(root.begin() + i);
        }
    }
}

std::vector<GameObject*> Scene::GetRootList() const
{
    return root;
}
#include "scene.h"
#include "gameobject.h"

void Scene::AddGameObject(GameObject* gameObject)
{
    assert(gameObject->GetIsSceneRoot() == false);

    // if parent != null, remove from parent first
    if (gameObject->GetParent() != nullptr)
    {
        gameObject->GetParent()->RemoveChild(gameObject);
    }

    gameObject->SetIsSceneRoot(true);
    root.push_back(gameObject);
}

void Scene::RemoveGameObject(GameObject* gameObject)
{
    assert(gameObject->GetParent() == nullptr);

    for (int i = root.size() - 1; i >= 0; i--)
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
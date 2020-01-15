#pragma once
#include "common.h"

class GameObject;
class Scene
{
public:
    void AddGameObject(GameObject* gameObject);
    void RemoveGameObject(GameObject* gameObject);
    std::vector<GameObject*> GetRootList() const;
private:
    std::vector<GameObject*> root;
};
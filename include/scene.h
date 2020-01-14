#pragma once
#include "common.h"

class GameObject;
class Scene
{
public:
    void AddGameObject(std::shared_ptr<GameObject> gameObject);
    void RemoveGameObject(std::shared_ptr<GameObject> gameObject);

private:
    std::vector<std::shared_ptr<GameObject>> root;
};
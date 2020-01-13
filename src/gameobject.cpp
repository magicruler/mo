#include "gameobject.h"
#include "gameobject_system.h"

std::shared_ptr<GameObject> GameObject::Create()
{
    GameObjectSystem *gameObjectSystem = GameObjectSystem::GetInstance();
    auto newGameObject = std::make_shared<GameObject>();
    gameObjectSystem->AddGameObject(newGameObject);

    return newGameObject;
}
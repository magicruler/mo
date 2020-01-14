#include "gameobject.h"
#include "gameobject_system.h"

unsigned int GameObject::IdCounter = 0;

std::shared_ptr<GameObject> GameObject::Create()
{
    GameObjectSystem *gameObjectSystem = GameObjectSystem::GetInstance();
    auto newGameObject = std::make_shared<GameObject>();
    gameObjectSystem->AddGameObject(newGameObject);

    return newGameObject;
}

void GameObject::RecalculateTransform()
{
    if (isDirty)
    {
        transform = glm::translate(glm::mat4(1.0f), this->position);
        transform = glm::scale(transform, scale);
        transform = glm::eulerAngleXYZ(rotation.x, rotation.y, rotation.z) * transform;

        if (parent != nullptr)
        {
            transform = parent->transform * transform;
        }

        for (size_t i = 0; i < children.size(); i++)
        {
            children[i]->isDirty = true;
            children[i]->RecalculateTransform();
        }

        isDirty = false;
    }
}

void GameObject::SetPosition(glm::vec3 position)
{
    this->position = position;
    isDirty = true;
}

glm::vec3 GameObject::GetPosition() const
{
    return position;
}

void GameObject::SetRotation(glm::vec3 rotation)
{
    this->rotation = rotation;
    isDirty = true;
}

glm::vec3 GameObject::GetRoation() const
{
    return rotation;
}

void GameObject::SetScale(glm::vec3 scale)
{
    this->scale = scale;
    isDirty = true;
}

glm::vec3 GameObject::GetScale() const
{
    return scale;
}

glm::mat4 GameObject::GetTransform() 
{
    if(isDirty)
    {
        this->RecalculateTransform();
    }
    return transform;
}
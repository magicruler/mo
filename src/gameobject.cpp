#include "gameobject.h"
#include "gameobject_system.h"
#include "component.h"

unsigned int GameObject::IdCounter = 0;

GameObject::GameObject()
{
    IdCounter++;
    ID = IdCounter;
}

void GameObject::Destroy(std::shared_ptr<GameObject> gameobject)
{
    gameobject->needDestroy = true;
}

std::shared_ptr<GameObject> GameObject::Create()
{
    GameObjectSystem *gameObjectSystem = GameObjectSystem::GetInstance();
    auto newGameObject = std::make_shared<GameObject>();
    gameObjectSystem->AddGameObject(newGameObject);

    return newGameObject;
}

void GameObject::AddComponent(std::shared_ptr<Component> component)
{
    component->gameObject = std::dynamic_pointer_cast<GameObject>(shared_from_this());
    size_t typeHashCode = component->GetHashID();
    components[typeHashCode] = component;
}

void GameObject::RemoveComponent(std::shared_ptr<Component> component)
{
    size_t typeHashCode = component->GetHashID();
    if (components[typeHashCode] == component)
    {
        component->gameObject = nullptr;
        components.erase(typeHashCode);
    }
    else
    {
        spdlog::info("Try to remove component which is not attached in this gameobject");
    }
}

void GameObject::RemoveAllComponent()
{
    for (auto const &pair : components)
    {
        pair.second->gameObject = nullptr;
    }

    components.clear();
}

void GameObject::Clear()
{
    RemoveAllComponent();
    RemoveAllChildren();
    parent = nullptr;
}

std::shared_ptr<GameObject> GameObject::GetParent() const
{
    return parent;
}

void GameObject::AddChild(std::shared_ptr<GameObject> child)
{
    child->parent = std::dynamic_pointer_cast<GameObject>(shared_from_this());
    children.push_back(child);
}

void GameObject::RemoveChild(size_t index)
{
    if (index >= children.size())
    {
        spdlog::info("Try to remove a non-exist child");
        return;
    }
    children[index]->parent = nullptr;
    children.erase(children.begin() + index);
}

void GameObject::RemoveAllChildren()
{
    for (auto &child : children)
    {
        child->parent = nullptr;
    }

    children.clear();
}

std::vector<std::shared_ptr<GameObject>> GameObject::GetChildren()
{
    return children;
}

std::shared_ptr<GameObject> GameObject::GetChild(size_t index)
{
    return children[index];
}

size_t GameObject::GetChildCount() const
{
    return children.size();
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
    if (isDirty)
    {
        this->RecalculateTransform();
    }
    return transform;
}
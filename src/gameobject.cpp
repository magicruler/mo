#include "gameobject.h"
#include "gameobject_system.h"
#include "component.h"

unsigned int GameObject::IdCounter = 0;

GameObject::GameObject()
{
    IdCounter++;
    ID = IdCounter;
}

void GameObject::Destroy(GameObject *gameObject, bool recursive)
{
    gameObject->needDestroy = true;
    if (recursive)
    {
        for (auto child : gameObject->children)
        {
            GameObject::Destroy(child);
        }
    }
}

GameObject *GameObject::Create()
{
    GameObjectSystem *gameObjectSystem = GameObjectSystem::GetInstance();
    auto newGameObject = new GameObject();
    gameObjectSystem->AddGameObject(newGameObject);

    return newGameObject;
}

void GameObject::AddComponent(Component *component)
{
    component->gameObject = this;
    size_t typeHashCode = component->GetHashID();
    components[typeHashCode] = component;
}

void GameObject::RemoveComponent(Component *component)
{
    size_t typeHashCode = component->GetHashID();
    if (components[typeHashCode] == component)
    {
        components.erase(typeHashCode);
        Component::Destroy(component);
    }
    else
    {
        spdlog::info("Try to remove component which is not attached in this gameobject");
    }
}

void GameObject::RemoveAllComponent()
{
    // delete all components
    for (auto const &pair : components)
    {
        Component::Destroy(pair.second);
    }

    components.clear();
}

void GameObject::Clear()
{
    RemoveAllComponent();
    RemoveAllChildren();
    parent = nullptr;
}

GameObject *GameObject::GetParent() const
{
    return parent;
}

void GameObject::AddChild(GameObject *child)
{
    child->parent = this;
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

std::vector<GameObject *> GameObject::GetChildren()
{
    return children;
}

GameObject *GameObject::GetChild(size_t index)
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
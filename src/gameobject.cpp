#include "gameobject.h"
#include "gameobject_system.h"
#include "component.h"
#include "scene_system.h"
#include "scene.h"

unsigned int GameObject::IdCounter = 0;

GameObject::GameObject()
{
    parent = nullptr;
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    transform = glm::mat4(1.0f);

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
    // non recursive
    else
    {
        // add direct children into current scene
        for (int i = gameObject->GetChildren().size() - 1; i >= 0; i--)
        {
            SceneSystem::GetInstance()->GetActivatedScene()->AddGameObject(gameObject->GetChildren()[i]);
        }
    }
}

GameObject *GameObject::Create(GameObject* parent)
{
    GameObjectSystem *gameObjectSystem = GameObjectSystem::GetInstance();
    auto newGameObject = new GameObject();
    newGameObject->SetName("NewGameObject");

    // Use GameObject System To Manage And Track All GameObjects
    gameObjectSystem->AddGameObject(newGameObject);

    // if no parent provided, directly attach on current scene
    if (parent == nullptr)
    {
        SceneSystem::GetInstance()->GetActivatedScene()->AddGameObject(newGameObject);
    }
    else
    {
        parent->AddChild(newGameObject);
    }

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
    if (isSceneRoot)
    {
        SceneSystem::GetInstance()->GetActivatedScene()->RemoveGameObject(this);
    }
    //
    if (parent != nullptr)
    {
        parent->RemoveChild(this);
    }
}

unsigned int GameObject::GetID() const
{
    return ID;
}

GameObject *GameObject::GetParent() const
{
    return parent;
}

void GameObject::AddChild(GameObject *child)
{
    // if child in scene root, first remove 
    if (child->isSceneRoot)
    {
        SceneSystem::GetInstance()->GetActivatedScene()->RemoveGameObject(child);
    }

    // if child already has a parent, remove from this parent
    if (child->parent != nullptr)
    {
        child->parent->RemoveChild(child);
    }

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

void GameObject::RemoveChild(GameObject* gameObject)
{
    for (int i = children.size() - 1; i >= 0; i--)
    {
        auto child = children[i];
        if (child == gameObject)
        {
            child->parent = nullptr;
            children.erase(children.begin() + i);
        }
    }
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

void GameObject::SetIsSceneRoot(bool val)
{
    isSceneRoot = val;
}
bool GameObject::GetIsSceneRoot()
{
    return isSceneRoot;
}


std::string GameObject::GetName() const
{
    return name;
}

void GameObject::SetName(std::string name)
{
    this->name = name;
}
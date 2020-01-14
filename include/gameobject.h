#pragma once
#include "common.h"
#include "component.h"

class GameObjectSystem;

class GameObject : public std::enable_shared_from_this<GameObject>, public Object
{
    MO_OBJECT("GameObject")

    friend class GameObjectSystem;

public:
    GameObject()
    {
        IdCounter++;
        ID = IdCounter;
    }

    static std::shared_ptr<GameObject> Create();

    static void Destroy(std::shared_ptr<GameObject> gameobject)
    {
        gameobject->needDestroy = true;
    }

    void AddComponent(std::shared_ptr<Component> component)
    {
        component->gameObject = shared_from_this();
        size_t typeHashCode = component->GetHashID();
        components[typeHashCode] = component;
    }

    template <typename T>
    void RemoveComponent()
    {
        T com = GetComponent<T>();
        if (com != nullptr)
        {
            RemoveComponent(com);
        }
    }

    void RemoveComponent(std::shared_ptr<Component> component)
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

    template <typename T>
    std::shared_ptr<T> GetComponent()
    {
        size_t typeHashCode = T::GetHashIDStatic();
        std::map<size_t, std::shared_ptr<Component>>::iterator it = components.find(typeHashCode);
        if (it != components.end())
        {
            return std::dynamic_pointer_cast<T>(it->second);
        }
        return nullptr;
    }

    void RemoveAllComponent()
    {
        for (auto const &pair : components)
        {
            pair.second->gameObject = nullptr;
        }

        components.clear();
    }

    void Clear()
    {
        RemoveAllComponent();
        RemoveAllChildren();
        parent = nullptr;
    }

    // Node Traits
public:
    std::shared_ptr<GameObject> GetParent() const
    {
        return parent;
    }

    void AddChild(std::shared_ptr<GameObject> child)
    {
        child->parent = shared_from_this();
        children.push_back(child);
    }

    void RemoveChild(size_t index)
    {
        if (index >= children.size())
        {
            spdlog::info("Try to remove a non-exist child");
            return;
        }
        children[index]->parent = nullptr;
        children.erase(children.begin() + index);
    }

    void RemoveAllChildren()
    {
        for (auto &child : children)
        {
            child->parent = nullptr;
        }

        children.clear();
    }

    std::vector<std::shared_ptr<GameObject>> GetChildren()
    {
        return children;
    }

    std::shared_ptr<GameObject> GetChild(size_t index)
    {
        return children[index];
    }

    size_t GetChildCount() const
    {
        return children.size();
    }

    void SetPosition(glm::vec3 position);
    glm::vec3 GetPosition() const;

    void SetRotation(glm::vec3 rotation);
    glm::vec3 GetRoation() const;

    void SetScale(glm::vec3 scale);
    glm::vec3 GetScale() const;

    glm::mat4 GetTransform();

private:

    void RecalculateTransform();

    bool needDestroy = false;
    std::map<size_t, std::shared_ptr<Component>> components;

    unsigned int ID;
    static unsigned int IdCounter;

    // Node Traits
private:
    std::shared_ptr<GameObject> parent;
    std::vector<std::shared_ptr<GameObject>> children;
    glm::mat4 transform;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale = glm::vec3(1.0f);
    bool isDirty = false;
};

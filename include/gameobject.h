#pragma once
#include "common.h"

class Component;
class GameObjectSystem;

class GameObject : public Object
{
    MO_OBJECT("GameObject")

    friend class GameObjectSystem;

public:
    GameObject();

    static std::shared_ptr<GameObject> Create();

    static void Destroy(std::shared_ptr<GameObject> gameobject);

    void AddComponent(Component *component);

    template <typename T>
    void RemoveComponent()
    {
        T com = GetComponent<T>();
        if (com != nullptr)
        {
            RemoveComponent(com);
        }
    }

    void RemoveComponent(Component *component);

    template <typename T>
    T *GetComponent()
    {
        size_t typeHashCode = T::GetHashIDStatic();
        std::map<size_t, Component *>::iterator it = components.find(typeHashCode);
        if (it != components.end())
        {
            return (T *)(it->second);
        }
        return nullptr;
    }

    void RemoveAllComponent();

    void Clear();

    // Node Traits
public:
    std::shared_ptr<GameObject> GetParent() const;
    void AddChild(std::shared_ptr<GameObject> child);
    void RemoveChild(size_t index);
    void RemoveAllChildren();

    std::vector<std::shared_ptr<GameObject>> GetChildren();
    std::shared_ptr<GameObject> GetChild(size_t index);
    size_t GetChildCount() const;

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
    std::map<size_t, Component *> components;

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

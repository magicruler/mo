#pragma once
#include "common.h"

class Component;

class ComponentSystem
{
public:
    static void Init();
    static void Destroy();

    static ComponentSystem *GetInstance();

    void AddComponent(Component* component);
    void RemoveComponent(Component* component);

    void Update();

private:
    ComponentSystem();

    ~ComponentSystem();

    static ComponentSystem *instance;
    std::map<size_t, std::vector<Component*>> componentsMap;
};
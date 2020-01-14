#pragma once
#include "common.h"

class Component;

class ComponentSystem
{
public:
    static void Init();

    static ComponentSystem *GetInstance();

    void AddComponent(std::shared_ptr<Component> component);

    void Update();

private:
    ComponentSystem();

    ~ComponentSystem();

    static ComponentSystem *instance;
    std::map<size_t, std::vector<std::shared_ptr<Component>>> componentsMap;
};
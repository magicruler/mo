#include "component_system.h"
#include "component.h"

ComponentSystem *ComponentSystem::instance = nullptr;

ComponentSystem::ComponentSystem()
{
}

ComponentSystem::~ComponentSystem()
{
    delete instance;
}

void ComponentSystem::Init()
{
    instance = new ComponentSystem();
}

ComponentSystem *ComponentSystem::GetInstance()
{
    assert(instance != nullptr);
    return instance;
}

void ComponentSystem::AddComponent(std::shared_ptr<Component> component)
{
    size_t typeHashCode = component->GetHashID();
    std::map<size_t, std::vector<std::shared_ptr<Component>>>::iterator it = componentsMap.find(typeHashCode);
    // already has this type, add into, else create a list, and add into
    if (it != componentsMap.end())
    {
        it->second.push_back(component);
    }
    else
    {
        componentsMap[typeHashCode] = std::vector<std::shared_ptr<Component>>();
        componentsMap[typeHashCode].push_back(component);
    }
}

void ComponentSystem::Update()
{
    for (auto &pair : componentsMap)
    {
        auto &list = pair.second;
        for (int i = list.size() - 1; i >= 0; i--)
        {
            if (list[i]->gameObject == nullptr)
            {
                list.erase(list.begin() + i);
            }
        }
    }
}
#include "component.h"
#include "component_system.h"

unsigned int Component::IdCounter = 0;

Component::Component()
{
    IdCounter++;
    ID = IdCounter;
}

void Component::Destroy(Component *component)
{
    ComponentSystem::GetInstance()->RemoveComponent(component);
    delete component;
}
#pragma once

#include "common.h"

class GameObject;
class ComponentSystem;

class Component : public Object
{
    MO_OBJECT("Component")
    friend class ComponentSystem;

public:
    Component()
    {
        IdCounter++;
        ID = IdCounter;
    }
    std::shared_ptr<GameObject> gameObject;

private:
    static unsigned int IdCounter;
    unsigned int ID;
    bool needDestroy = false;
};

unsigned int Component::IdCounter = 0;
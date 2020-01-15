#pragma once

#include "common.h"

class GameObject;
class ComponentSystem;

class Component : public Object
{
    MO_OBJECT("Component")
    friend class ComponentSystem;

public:
    Component();
    GameObject* gameObject;
    static void Destroy(Component* component);

private:
    static unsigned int IdCounter;
    unsigned int ID;
    bool needDestroy = false;
};


#pragma once
#include "common.h"

class Actor;

class Component
{
    MO_OBJECT("Component")

public:
    Component()
    {
    }

    void SetParent(Actor* parent)
    {
        this->parent = parent;
    }

    Actor* GetParent() const
    {
        return parent;
    }
    
    virtual void Clear()
    {

    }

private:
    Actor* parent = nullptr;
};
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

    Actor* GetOwner() const
    {
        return parent;
    }
    
    virtual void Clear()
    {

    }

    /*
    Called When Attached To Actor
    */
    virtual void Attach()
    {
    }

private:
    Actor* parent = nullptr;
};
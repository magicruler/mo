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

    void SetEnable(bool val)
    {
        enable = val;
    }
    
    bool GetEnable() const
    {
        return enable;
    }

private:
    bool enable = true;
    Actor* parent = nullptr;
};
#pragma once
#include "common.h"

class ResourceSystem
{
public:
    static void Init()
    {
        instance = new ResourceSystem();
    }

    static ResourceSystem *GetInstance()
    {
        assert(instance != nullptr);
        return instance;
    }

private:
    ResourceSystem()
    {
    }

    ~ResourceSystem()
    {
        delete instance;
    }

    static ResourceSystem *instance;
};

ResourceSystem *ResourceSystem::instance = nullptr;
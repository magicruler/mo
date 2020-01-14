#pragma once
#include "common.h"

class ResourceSystem
{
public:
    static void Init();

    static ResourceSystem *GetInstance();

private:
    ResourceSystem();
    ~ResourceSystem();

    static ResourceSystem *instance;
};

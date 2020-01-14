#include "resource_system.h"

ResourceSystem *ResourceSystem::instance = nullptr;

ResourceSystem::ResourceSystem()
{
}

ResourceSystem::~ResourceSystem()
{
    delete instance;
}



void ResourceSystem::Init()
{
    instance = new ResourceSystem();
}

ResourceSystem *ResourceSystem::GetInstance()
{
    assert(instance != nullptr);
    return instance;
}

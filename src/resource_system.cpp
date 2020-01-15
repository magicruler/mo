#include "resource_system.h"

ResourceSystem *ResourceSystem::instance = nullptr;

ResourceSystem::ResourceSystem()
{
}

ResourceSystem::~ResourceSystem()
{
}

void ResourceSystem::Init()
{
    instance = new ResourceSystem();
}

void ResourceSystem::Destroy()
{
    delete instance;
}

ResourceSystem *ResourceSystem::GetInstance()
{
    assert(instance != nullptr);
    return instance;
}

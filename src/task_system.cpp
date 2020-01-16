#include "task_system.h"

TaskSystem* TaskSystem::instance = nullptr;

TaskSystem::TaskSystem()
{
}

TaskSystem::~TaskSystem()
{
}

void TaskSystem::Init()
{
    assert(instance == nullptr);
    instance = new TaskSystem();
}

void TaskSystem::Destroy()
{
    delete instance;
}

TaskSystem* TaskSystem::GetInstance()
{
    assert(instance != nullptr);
    return instance;
}
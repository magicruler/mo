#pragma once
#include "common.h"

template <typename T>
class Task
{
public:
    Task(std::function<T()> func)
    {
        action = func;
    }

private:
    std::function<T()> action;
};

class Thread 
{
    Thread()
    {

    }
private:
    std::thread systemThread;
};

class TaskSystem
{
public:
    static void Init();
    static void Destroy();
    static TaskSystem* GetInstance();


private:
    TaskSystem();
    ~TaskSystem();
    static TaskSystem* instance;
};
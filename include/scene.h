#pragma once
#include "common.h"

class Actor;

class Scene
{
public:
    Scene();
    ~Scene();
    Actor* rootNode = nullptr;
    
    void Tick();

    std::string GetName()
    {
        return name;
    }

    void SetName(const std::string newName)
    {
        name = newName;
    }

private:
    std::string name = "";
};
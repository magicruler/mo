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
};
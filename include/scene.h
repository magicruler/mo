#pragma once
#include "common.h"

class Actor;
class Camera;

class Scene
{
public:
    Scene();
    ~Scene();
    Actor* rootNode = nullptr;
    
    void Tick();

    std::vector<Camera*> GetCameras();
    std::vector<Actor*> GetRenderables();

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
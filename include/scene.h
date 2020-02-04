#pragma once
#include "common.h"

class Actor;
class Camera;
class Light;

class Scene
{
public:
    Scene();
    ~Scene();
    Actor* rootNode = nullptr;
    
    void Tick();

    std::vector<Camera*> GetCameras();
    std::vector<Actor*> GetRenderables();
    std::vector<Light*> GetLights();

    std::string GetName()
    {
        return name;
    }

    void SetName(const std::string newName)
    {
        name = newName;
    }

    void SetAmbient(glm::vec3& color)
    {
        ambientColor = color;
    }

    glm::vec3 GetAmbient()
    {
        return ambientColor;
    }

private:
    std::string name = "";
    glm::vec3 ambientColor = glm::vec3(0.1f, 0.1f, 0.1f);
};
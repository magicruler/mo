#pragma once
#include "common.h"
#include "actor.h"

class Light: public Actor
{
    MO_OBJECT("Light")
public:
    void SetLightIntensity(float intensity)
    {
        lightIntensity = intensity;
    }

    void SetLightColor(glm::vec3& color)
    {
        lightColor = color;
    }

    glm::vec3 GetLightIntensityColor() const
    {
        return lightIntensity * lightColor;
    }

private:
    float lightIntensity = 1.0f;
    glm::vec3 lightColor = glm::vec3(0.7f, 0.7f, 0.7f);
};
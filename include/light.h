#pragma once
#include "common.h"
#include "component.h"

enum class LightType
{
    Point = 0,
    Spot = 1,
    Directional = 2
};

class GPUBuffer;

class Light: public Component
{
    MO_OBJECT("Light")
public:
    virtual void Clear();

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

    void SetLightType(LightType lightType)
    {
        this->lightType = lightType;
    }

    LightType GetLightType() const 
    {
        return this->lightType;
    }

    void SetCastShadow(bool val);

    bool GetCastShadow()
    {
        return this->castShadow;
    }

    glm::mat4 GetLightProjection() const
    {
        return glm::perspective(spotAngle * 2.0f + spotEdgeAngle * 2.0f, 1.0f, 0.1f, 1000.0f);
    }

    /*
    Spot Only
    */
    void SetSpotDir(const glm::vec3& spotDir)
    {
        this->spotDir = spotDir;
    }

    glm::vec3 GetSpotDir() const
    {
        return spotDir;
    }

    /*
    spotAngle: Degree
    */
    void SetSpotAngle(float spotAngle)
    {
        this->spotAngle = glm::radians(spotAngle);
    }

    float GetSpotAngle() const
    {
        return this->spotAngle;
    }

    void SetSpotEdgeAngle(float spotEdgeAngle)
    {
        this->spotEdgeAngle = glm::radians(spotEdgeAngle);
    }

    float GetSpotEdgeAngle() const
    {
        return this->spotEdgeAngle;
    }

    void SetShadowUniformBlock();

    GPUBuffer* GetShadowUniformBlock() const
    {
        return shadowUniformBlock;
    }

private:
    
    LightType lightType = LightType::Point;
    float lightIntensity = 1.0f;
    glm::vec3 lightColor = glm::vec3(0.7f, 0.7f, 0.7f);
    bool castShadow = false;

    GPUBuffer* shadowUniformBlock = nullptr;

    /*
    Spot Light Only
    */
    glm::vec3 spotDir = glm::vec3(0.0f, 0.0f, -1.0f);
    float spotAngle = 0.0f;
    float spotEdgeAngle = glm::radians(10.0f);
};
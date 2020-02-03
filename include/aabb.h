#pragma once
#include "common.h"

struct AABB
{
    AABB()
    {
        minPos = glm::vec3(999999999999999.0f, 999999999999999.0f, 999999999999999.0f);
        maxPos = glm::vec3(-999999999999999.0f, -999999999999999.0f, -999999999999999.0f);
    }

    AABB(glm::vec3 minP, glm::vec3 maxP)
    {
        minPos = minP;
        maxPos = maxP;
    }

    glm::vec3 operator[](int index)
    {
        if (index == 0)
        {
            return minPos;
        }
        else
        {
            return maxPos;
        }
    }

    void Append(AABB& other)
    {
        minPos = glm::vec3(Math::Min(minPos.x, other.minPos.x), Math::Min(minPos.y, other.minPos.y), Math::Min(minPos.z, other.minPos.z));
        maxPos = glm::vec3(Math::Max(maxPos.x, other.maxPos.x), Math::Max(maxPos.y, other.maxPos.y), Math::Max(maxPos.z, other.maxPos.z));
    }

    glm::vec3 GetCenter()
    {
        return (minPos + maxPos) * 0.5f;
    }

    float GetHalfLength()
    {
        return glm::length(maxPos - minPos) * 0.5f;
    }

    glm::vec3 minPos;
    glm::vec3 maxPos;
};

struct TransformedAABB
{
    void ComputeBounding()
    {
        glm::vec3 a1 = transformation * glm::vec4(originalAABB.minPos.x, originalAABB.minPos.y, originalAABB.minPos.z, 1.0f);
        glm::vec3 a2 = transformation * glm::vec4(originalAABB.maxPos.x, originalAABB.minPos.y, originalAABB.minPos.z, 1.0f);
        glm::vec3 a3 = transformation * glm::vec4(originalAABB.minPos.x, originalAABB.maxPos.y, originalAABB.minPos.z, 1.0f);
        glm::vec3 a4 = transformation * glm::vec4(originalAABB.minPos.x, originalAABB.minPos.y, originalAABB.maxPos.z, 1.0f);
        glm::vec3 a5 = transformation * glm::vec4(originalAABB.minPos.x, originalAABB.maxPos.y, originalAABB.maxPos.z, 1.0f);
        glm::vec3 a6 = transformation * glm::vec4(originalAABB.maxPos.x, originalAABB.minPos.y, originalAABB.maxPos.z, 1.0f);
        glm::vec3 a7 = transformation * glm::vec4(originalAABB.maxPos.x, originalAABB.maxPos.y, originalAABB.minPos.z, 1.0f);
        glm::vec3 a8 = transformation * glm::vec4(originalAABB.maxPos.x, originalAABB.maxPos.y, originalAABB.maxPos.z, 1.0f);
       
        std::vector<glm::vec3> points({a1, a2, a3, a4, a5, a6, a7, a8});
        
        float minX = 999999999999999.0f;
        float minY = 999999999999999.0f;
        float minZ = 999999999999999.0f;
        float maxX = -999999999999999.0f;
        float maxY = -999999999999999.0f;
        float maxZ = -999999999999999.0f;

        for (auto& point : points)
        {
            minX = Math::Min(minX, point.x);
            minY = Math::Min(minY, point.y);
            minZ = Math::Min(minZ, point.z);

            maxX = Math::Max(maxX, point.x);
            maxY = Math::Max(maxY, point.y);
            maxZ = Math::Max(maxZ, point.z);
        }

        bounding.minPos = glm::vec3(minX, minY, minZ);
        bounding.maxPos = glm::vec3(maxX, maxY, maxZ);
    }
    
    AABB bounding;
    AABB originalAABB;
    glm::mat4 transformation;
};
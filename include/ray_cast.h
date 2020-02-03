#pragma once
#include "common.h"
#include "actor.h"

struct Ray
{
	glm::vec3 origin = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 direction = glm::vec3(0.0f, 0.0f, 0.0f);
};

struct RayCastInteraction
{
	Actor* target = nullptr;
	float distance = 9999999999.0f;
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
};

namespace Physics
{
	void RayCast(Ray ray, LAYER_MASK layerMask, RayCastInteraction& interaction);
};
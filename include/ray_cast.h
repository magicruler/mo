#pragma once
#include "common.h"
#include "actor.h"

struct RayCastInteraction
{
	Actor* target = nullptr;
	float distance = 9999999999.0f;
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
};

namespace Physics
{
	void RayCast(glm::vec3 origin, glm::vec3 direction, LAYER_MASK layerMask, RayCastInteraction& interaction);
};
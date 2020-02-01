#pragma once
#include "common.h"
#include "actor.h"

class RenderTarget;

class Camera: public Actor
{
	MO_OBJECT("Camera")
public:
	virtual void Tick();
	glm::mat4 GetProjection();
	glm::mat4 GetViewMatrix();

	float fov = 45.0f;
	float ratio = 1024.0f / 768.0f;
	float nearPlane = 0.1f;
	float farPlane = 10000.0f;

	RenderTarget* renderTarget = nullptr;
};
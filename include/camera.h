#pragma once
#include "common.h"
#include "component.h"

class RenderTarget;
class GPUBuffer;
struct Ray;

class Camera: public Component
{
	MO_OBJECT("Camera")
public:
	Camera();

	virtual void Clear();
	void Render();

	void PreRender();

	glm::mat4 GetProjection();
	glm::mat4 GetRenderTargetProjection();

	glm::mat4 GetViewMatrix();

	GPUBuffer* GetUniformBlock()
	{
		return uniformBlock;
	}

	Ray ScreenRay(float screenX, float screenY);

	float fov = 45.0f;
	float ratio = 1024.0f / 768.0f;
	float nearPlane = 0.3f;
	float farPlane = 3000.0f;

	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	unsigned int cullingMask = EVERY_THING;

    glm::vec4 clearColor = glm::vec4(49.0f/255.0f, 77.0f/255.0f, 121.0f/255.0f, 1.0f);
	RenderTarget* renderTarget = nullptr;

private:
	GPUBuffer* uniformBlock;
};
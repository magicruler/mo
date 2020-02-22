#pragma once
#include "common.h"
#include "component.h"

class RenderTarget;
class GPUBuffer;
struct Ray;
class Event;
class Material;
class DeferredPipeline;

class Camera: public Component
{
	MO_OBJECT("Camera")
public:
	Camera();
	~Camera();
	
	virtual void Clear();
	virtual void OnNotify(const Event& e);
	void Render();

	void PreRender();

	void SetRenderTarget(RenderTarget* renderTarget);

	RenderTarget* GetRenderTarget() const
	{
		return renderTarget;
	}

	RenderTarget* GetHdrTarget() const
	{
		return hdrTarget;
	}

	Material* GetPostProcessingMaterial() const
	{
		return postProcessingMaterial;
	}

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
	bool hasPostProcessing = true;

private:
	Material* postProcessingMaterial = nullptr;
	RenderTarget* hdrTarget = nullptr;
	RenderTarget* renderTarget = nullptr;
	GPUBuffer* uniformBlock = nullptr;
	DeferredPipeline* pipeline = nullptr;
};
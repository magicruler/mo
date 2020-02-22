#pragma once
#include "common.h"

class Camera;
class RenderTarget;

class DeferredPipeline
{
public:
	void Init(Camera* camera);

	void Render();

	void Resize(glm::vec2 size);

private:
	Camera* camera = nullptr;
	RenderTarget* gBuffer = nullptr;
};
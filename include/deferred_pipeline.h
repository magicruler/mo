#pragma once
#include "common.h"

class Camera;
class RenderTarget;
class Texture;
class Material;

class DeferredPipeline
{
public:
	void Init(Camera* camera);

	void Render();

	void RenderDeferredPass();
	void RenderForwardPass();

	void Resize(const glm::vec2& size);

	Texture* GetPositionTexture() const;
	Texture* GetNormalMetalnessTexture() const;
	Texture* GetAlbedoSpecularTexture() const;
	Texture* GetDepthTexture() const;

private:

	Material* lightPassMaterial = nullptr;

	Camera* camera = nullptr;
	RenderTarget* gBuffer = nullptr;
	RenderTarget* lightPass = nullptr;
};
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
	void RenderDebugPass();

	void Resize(const glm::vec2& size);

	Texture* GetPositionTexture() const;
	Texture* GetNormalMetalnessTexture() const;
	Texture* GetAlbedoRoughnessTexture() const;
	Texture* GetDepthTexture() const;

	// 0 ---- without debug
	int debugOption = 0;

private:
	Material* lightPassMaterial = nullptr;
	Material* gbufferDebugMaterial = nullptr;

	Camera* camera = nullptr;
	RenderTarget* gBuffer = nullptr;
	RenderTarget* lightPass = nullptr;
};
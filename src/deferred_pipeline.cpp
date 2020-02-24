#include "deferred_pipeline.h"
#include "camera.h"
#include "render_target.h"
#include "game.h"
#include "scene.h"
#include "component_manager.h"
#include "mesh_component.h"
#include "command_buffer.h"
#include "actor.h"
#include "mesh.h"
#include "material.h"
#include "resources.h"
#include "light.h"

void DeferredPipeline::Init(Camera* camera)
{
	this->camera = camera;
	assert(this->camera->GetRenderTarget() != nullptr);

	lightPassMaterial = Resources::GetMaterial("lightPass.json");
	gbufferDebugMaterial = Resources::GetMaterial("gbufferDebug.json");
	renderTargetBlitMaterial = Resources::GetMaterial("renderTargetBlit.json");

	// CreateGBuffer
	std::vector<RenderTargetDescriptor> gBufferDescriptors;
	
	// Position
	RenderTargetDescriptor gBufferAttachment0Descriptor = RenderTargetDescriptor();
	gBufferAttachment0Descriptor.format = RENDER_TARGET_FORMAT::RGB32F;

	// Normal Metalness
	RenderTargetDescriptor gBufferAttachment1Descriptor = RenderTargetDescriptor();
	gBufferAttachment1Descriptor.format = RENDER_TARGET_FORMAT::RGBA16F;

	// Albedo Specular
	RenderTargetDescriptor gBufferAttachment2Descriptor = RenderTargetDescriptor();
	gBufferAttachment2Descriptor.format = RENDER_TARGET_FORMAT::RGBA16F;

	gBufferDescriptors.push_back(gBufferAttachment0Descriptor);
	gBufferDescriptors.push_back(gBufferAttachment1Descriptor);
	gBufferDescriptors.push_back(gBufferAttachment2Descriptor);

	gBuffer = new RenderTarget(this->camera->GetRenderTarget()->GetSize().x, this->camera->GetRenderTarget()->GetSize().y, gBufferDescriptors);

	// Create Light Pass
	std::vector<RenderTargetDescriptor> lightPassDescriptors;

	// Color
	RenderTargetDescriptor lightPassColorAttachment0Descriptor = RenderTargetDescriptor();
	lightPassColorAttachment0Descriptor.format = RENDER_TARGET_FORMAT::RGBA16F;

	lightPassDescriptors.push_back(lightPassColorAttachment0Descriptor);

	lightPass = new RenderTarget(this->camera->GetRenderTarget()->GetSize().x, this->camera->GetRenderTarget()->GetSize().y, lightPassDescriptors, false);
}

void DeferredPipeline::Resize(const glm::vec2& size)
{
	if (gBuffer != nullptr && lightPass != nullptr)
	{
		gBuffer->Resize(size.x, size.y);
		lightPass->Resize(size.x, size.y);
	}
}

void DeferredPipeline::Render()
{
	// Render Deferred Pass
	RenderDeferredPass();
	
	if (debugOption == 0)
	{
		// Render Forward Pass
		RenderForwardPass();
	}
	else
	{
		RenderDebugPass();
	}
}

void DeferredPipeline::RenderDeferredPass()
{
	Scene* currentScene = Game::ActiveSceneGetPointer();

	std::list<MeshComponent*> meshComponents = ComponentManager::GetInstance()->GetMeshComponents();
	std::list<Light*> lights = ComponentManager::GetInstance()->GetLightComponents();

	auto cb = Game::GetCommandBuffer();

	// G Buffer Rendering

	cb->SetRenderTarget(gBuffer);

	glm::vec2 renderTargetSize = gBuffer->GetSize();

	cb->SetViewport(glm::vec2(0.0f, 0.0f), glm::vec2(renderTargetSize.x, renderTargetSize.y));
	
	cb->SetClearDepth(1.0f);
	cb->SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));

	cb->Clear(CLEAR_BIT::COLOR | CLEAR_BIT::DEPTH);

	for (auto meshComponent : meshComponents)
	{
		if (camera->cullingMask != EVERY_THING)
		{
			if ((meshComponent->GetOwner()->GetLayerFlag() & camera->cullingMask) == 0)
			{
				continue;
			}
		}

		auto materials = meshComponent->materials;

		Mesh* mesh = meshComponent->mesh;
		assert(mesh != nullptr);

		glm::mat4 transformation = meshComponent->GetOwner()->GetLocalToWorldMatrix();

		for (int i = 0; i < mesh->children.size(); i++)
		{
			Material* material = nullptr;
			if (i >= materials.size())
			{
				material = materials[0];
			}
			else
			{
				material = materials[i];
			}

			if (material->GetPass() != MATERIAL_PASS::DEFERRED)
			{
				continue;
			}

			cb->RenderMesh(camera, material, mesh->children[i], transformation);
		}
	}

	// Light Pass Rendering

	// Draw On Light Pass
	cb->SetRenderTarget(lightPass);
	cb->SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
	cb->Clear(CLEAR_BIT::COLOR);

	// G Buffer Pass
	lightPassMaterial->SetTextureProperty("gBufferPosition", GetPositionTexture());
	lightPassMaterial->SetTextureProperty("gBufferNormalMetalness", GetNormalMetalnessTexture());
	lightPassMaterial->SetTextureProperty("gBufferAlbedoRoughness", GetAlbedoRoughnessTexture());

	// Light Uniforms
	glm::mat4 view = camera->GetViewMatrix();
	int index = 0;
	for (auto light : lights)
	{
		if (index >= 16)
		{
			break;
		}

		// Camera Space
		glm::vec3 lightPos = view * glm::vec4(light->GetOwner()->GetPosition(), 1.0f);

		lightPassMaterial->SetVector3("lights[" + std::to_string(index) + "].Color", light->GetLightIntensityColor());
		lightPassMaterial->SetVector3("lights[" + std::to_string(index) + "].Position", lightPos);

		index += 1;
	}

	for (int i = index; i <= 15; i++)
	{
		lightPassMaterial->SetVector3("lights[" + std::to_string(index) + "].Color", glm::vec3(0.0f));
		lightPassMaterial->SetVector3("lights[" + std::to_string(index) + "].Position", glm::vec3(0.0f));
	}

	cb->RenderQuad(glm::vec2(0.0f, 0.0f), renderTargetSize, camera->GetRenderTargetProjection(), lightPassMaterial);

	cb->Submit();
}

void DeferredPipeline::RenderForwardPass()
{
	RenderTarget* renderTarget = camera->GetRenderTarget();
	RenderTarget* hdrTarget = camera->GetHdrTarget();

	glm::vec2 renderTargetSize = renderTarget->GetSize();
	Scene* currentScene = Game::ActiveSceneGetPointer();

	std::list<MeshComponent*> meshComponents = ComponentManager::GetInstance()->GetMeshComponents();

	std::list<Light*> lights = ComponentManager::GetInstance()->GetLightComponents();

	auto cb = Game::GetCommandBuffer();

	if (camera->hasPostProcessing)
	{
		cb->SetRenderTarget(hdrTarget);
	}
	else
	{
		cb->SetRenderTarget(renderTarget);
	}

	cb->SetViewport(glm::vec2(0.0f, 0.0f), glm::vec2(renderTargetSize.x, renderTargetSize.y));
	
	cb->SetClearColor(camera->clearColor);
	cb->Clear(CLEAR_BIT::COLOR);

	cb->CopyDepthBuffer(gBuffer, (camera->hasPostProcessing?hdrTarget:renderTarget));

	// Draw Light Pass On Current Render Target
	cb->DisableDepth();
	renderTargetBlitMaterial->SetTextureProperty("renderTarget", lightPass->GetAttachmentTexture(0));
	cb->RenderQuad(glm::vec2(0.0f, 0.0f), renderTargetSize, camera->GetRenderTargetProjection(), renderTargetBlitMaterial);
	cb->EnableDepth();

	for (auto meshComponent : meshComponents)
	{
		if (camera->cullingMask != EVERY_THING)
		{
			if ((meshComponent->GetOwner()->GetLayerFlag() & camera->cullingMask) == 0)
			{
				continue;
			}
		}

		auto materials = meshComponent->materials;

		Mesh* mesh = meshComponent->mesh;
		assert(mesh != nullptr);

		glm::mat4 transformation = meshComponent->GetOwner()->GetLocalToWorldMatrix();

		for (int i = 0; i < mesh->children.size(); i++)
		{
			Material* material = nullptr;
			if (i >= materials.size())
			{
				material = materials[0];
			}
			else
			{
				material = materials[i];
			}

			if (material->GetPass() != MATERIAL_PASS::FORWARD)
			{
				continue;
			}

			cb->RenderMesh(camera, material, mesh->children[i], transformation);
		}
	}

	// Render skybox
	cb->RenderSkyBox(glm::mat4(glm::mat3(camera->GetViewMatrix())), camera->GetProjection());

	// Post Processing
	if (camera->hasPostProcessing)
	{
		cb->SetRenderTarget(renderTarget);
		cb->DisableDepth();

		camera->GetPostProcessingMaterial()->SetTextureProperty("hdrTarget", hdrTarget->GetAttachmentTexture(0));
		camera->GetPostProcessingMaterial()->SetFloat("exposure", 1.5f);

		cb->RenderQuad(glm::vec2(0.0f, 0.0f), renderTargetSize, camera->GetRenderTargetProjection(), camera->GetPostProcessingMaterial());
		cb->EnableDepth();
	}

	cb->Submit();
}

void DeferredPipeline::RenderDebugPass()
{
	if (gBuffer == nullptr)
	{
		return;
	}

	RenderTarget* renderTarget = camera->GetRenderTarget();
	RenderTarget* hdrTarget = camera->GetHdrTarget();

	glm::vec2 renderTargetSize = renderTarget->GetSize();
	Scene* currentScene = Game::ActiveSceneGetPointer();

	std::list<MeshComponent*> meshComponents = ComponentManager::GetInstance()->GetMeshComponents();

	std::list<Light*> lights = ComponentManager::GetInstance()->GetLightComponents();

	auto cb = Game::GetCommandBuffer();
	cb->SetRenderTarget(renderTarget);

	if (renderTarget->HasDepth())
	{
		cb->EnableDepth();
	}

	cb->SetViewport(glm::vec2(0.0f, 0.0f), glm::vec2(renderTargetSize.x, renderTargetSize.y));

	cb->DisableDepth();

	gbufferDebugMaterial->SetInt("debugOption", debugOption);
	gbufferDebugMaterial->SetTextureProperty("gBufferPosition", GetPositionTexture());
	gbufferDebugMaterial->SetTextureProperty("gBufferNormalMetalness", GetNormalMetalnessTexture());
	gbufferDebugMaterial->SetTextureProperty("gBufferAlbedoRoughness", GetAlbedoRoughnessTexture());
	gbufferDebugMaterial->SetTextureProperty("gBufferDepth", GetDepthTexture());

	gbufferDebugMaterial->SetTextureProperty("lightPass", lightPass->GetAttachmentTexture(0));

	cb->RenderQuad(glm::vec2(0.0f, 0.0f), renderTargetSize, camera->GetRenderTargetProjection(), gbufferDebugMaterial);
	cb->EnableDepth();

	cb->Submit();
}

Texture* DeferredPipeline::GetPositionTexture() const
{
	if (gBuffer != nullptr)
	{
		return gBuffer->GetAttachmentTexture(0);
	}

	return nullptr;
}

Texture* DeferredPipeline::GetNormalMetalnessTexture() const
{
	if (gBuffer != nullptr)
	{
		return gBuffer->GetAttachmentTexture(1);
	}

	return nullptr;
}

Texture* DeferredPipeline::GetAlbedoRoughnessTexture() const
{
	if (gBuffer != nullptr)
	{
		return gBuffer->GetAttachmentTexture(2);
	}

	return nullptr;
}

Texture* DeferredPipeline::GetDepthTexture() const
{
	if (gBuffer != nullptr)
	{
		return gBuffer->GetDepthTexture();
	}

	return nullptr;
}
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

void DeferredPipeline::Init(Camera* camera)
{
	this->camera = camera;
	assert(this->camera->GetRenderTarget() != nullptr);

	lightPassMaterial = Resources::GetMaterial("lightPass.json");
	
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
	RenderTargetDescriptor colorAttachment0Descriptor = RenderTargetDescriptor();
	colorAttachment0Descriptor.format = RENDER_TARGET_FORMAT::RGBA16F;

	lightPassDescriptors.push_back(colorAttachment0Descriptor);

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
	
	// Render Forward Pass
	RenderForwardPass();
}

void DeferredPipeline::RenderDeferredPass()
{
	Scene* currentScene = Game::ActiveSceneGetPointer();

	std::list<MeshComponent*> meshComponents = ComponentManager::GetInstance()->GetMeshComponents();
	std::list<Light*> lights = ComponentManager::GetInstance()->GetLightComponents();

	auto cb = Game::GetCommandBuffer();
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

	if (renderTarget->HasDepth())
	{
		cb->EnableDepth();
	}

	cb->SetViewport(glm::vec2(0.0f, 0.0f), glm::vec2(renderTargetSize.x, renderTargetSize.y));
	
	cb->SetClearColor(camera->clearColor);
	cb->Clear(CLEAR_BIT::COLOR);

	cb->CopyDepthBuffer(gBuffer, (camera->hasPostProcessing?hdrTarget:renderTarget));

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
		camera->GetPostProcessingMaterial()->SetFloat("exposure", 1.0f);

		cb->RenderQuad(glm::vec2(0.0f, 0.0f), renderTargetSize, camera->GetRenderTargetProjection(), camera->GetPostProcessingMaterial());
		cb->EnableDepth();
	}

	cb->Submit();
}

Texture* DeferredPipeline::GetPositionTexture() const
{
	if (gBuffer != nullptr)
	{
		gBuffer->GetAttachmentTexture(0);
	}

	return nullptr;
}

Texture* DeferredPipeline::GetNormalMetalnessTexture() const
{
	if (gBuffer != nullptr)
	{
		gBuffer->GetAttachmentTexture(1);
	}

	return nullptr;
}

Texture* DeferredPipeline::GetAlbedoSpecularTexture() const
{
	if (gBuffer != nullptr)
	{
		gBuffer->GetAttachmentTexture(2);
	}

	return nullptr;
}

Texture* DeferredPipeline::GetDepthTexture() const
{
	if (gBuffer != nullptr)
	{
		gBuffer->GetDepthTexture();
	}

	return nullptr;
}
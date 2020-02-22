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

void DeferredPipeline::Init(Camera* camera)
{
	this->camera = camera;


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
	RenderTarget* renderTarget = camera->GetRenderTarget();
	RenderTarget* hdrTarget = camera->GetHdrTarget();

	glm::vec2 renderTargetSize = renderTarget->GetSize();
	Scene* currentScene = Game::ActiveSceneGetPointer();
	camera->ratio = renderTargetSize.x / renderTargetSize.y;

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
	cb->SetClearDepth(1.0f);
	cb->SetClearColor(camera->clearColor);
	cb->Clear(CLEAR_BIT::COLOR | CLEAR_BIT::DEPTH);

	// Render Stuff
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
			if (i >= materials.size())
			{
				cb->RenderMesh(camera, materials[0], mesh->children[i], transformation);
			}
			else
			{
				cb->RenderMesh(camera, materials[i], mesh->children[i], transformation);
			}
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
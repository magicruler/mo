#include "camera.h"
#include "game.h"
#include "render_target.h"
#include "scene.h"

#include "material.h"
#include "mesh.h"
#include "vertex_array.h"

#include "time_manager.h"

#ifndef __glad_h_
#include <glad/glad.h>
#endif

#include "ray_cast.h"

#include "light.h"
#include "mesh_component.h"
#include "component_manager.h"
#include "actor.h"
#include "gpu_buffer.h"
#include "command_buffer.h"

#include "resources.h"
#include "event.h"

#include "deferred_pipeline.h"

struct CameraUniformBlock
{
	alignas(16) glm::mat4 projection;
	alignas(16) glm::mat4 view;
};

void Camera::Clear()
{
	ComponentManager::GetInstance()->AddToAvaliableCameraComponentsList(this);
	
	renderTarget = nullptr;

	if (hdrTarget != nullptr)
	{
		delete hdrTarget;
		hdrTarget = nullptr;
	}
}

Camera::Camera()
	:Component()
{
	renderTarget = nullptr;
	uniformBlock = new GPUBuffer();
	auto size = sizeof(CameraUniformBlock);
	uniformBlock->SetData(BUFFER_USAGE::UNIFORM, nullptr, size, BUFFER_DRAW_TYPE::STREAM_DRAW);
}

Camera::~Camera()
{
	if (hdrTarget != nullptr)
	{
		delete hdrTarget;
	}

	if (uniformBlock != nullptr)
	{
		delete uniformBlock;
	}

	if (pipeline != nullptr)
	{
		delete pipeline;
	}
}

void Camera::SetRenderTarget(RenderTarget* renderTarget)
{
	this->renderTarget = renderTarget;

	if (hdrTarget == nullptr && this->hasPostProcessing)
	{
		postProcessingMaterial = Resources::GetMaterial("postprocessing.json");

		auto size = this->renderTarget->GetSize();
		hdrTarget = new RenderTarget(size.x, size.y, GL_HALF_FLOAT, 1, true, false);
		this->Register(RenderTargetResizeEvent::GetHashIDStatic());
	}

	if (pipeline == nullptr)
	{
		pipeline = new DeferredPipeline();
		pipeline->Init(this);
	}
}

void Camera::OnNotify(const Event& e)
{
	if (e.GetHashID() == RenderTargetResizeEvent::GetHashIDStatic())
	{
		const RenderTargetResizeEvent& resizeEvent = dynamic_cast<const RenderTargetResizeEvent&>(e);
		if (resizeEvent.renderTarget == this->renderTarget)
		{
			hdrTarget->Resize(resizeEvent.size.x, resizeEvent.size.y);

			if (pipeline != nullptr)
			{
				pipeline->Resize(resizeEvent.size);
			}
		}
	}
}

glm::mat4 Camera::GetProjection()
{
	return glm::perspective(fov, ratio, nearPlane, farPlane);
}

glm::mat4 Camera::GetRenderTargetProjection()
{
	auto renderTargetSize = this->renderTarget->GetSize();
	return glm::ortho(0.0f, renderTargetSize.x, renderTargetSize.y, 0.0f, -1.0f, 1.0f);
}

/*
TODO, USE CACHED INVERSE MATRIX
*/
glm::mat4 Camera::GetViewMatrix()
{
	Actor* actor = this->GetOwner();
	glm::vec3 worldPos = actor->GetPosition();
	return glm::lookAt(worldPos, worldPos + cameraFront, cameraUp);
}

void Camera::PreRender()
{
	if (!GetEnable())
	{
		return;
	}

	// Write Uniform Buffer
	CameraUniformBlock uniformBlockData;
	uniformBlockData.projection = GetProjection();
	uniformBlockData.view = GetViewMatrix();
	
	uniformBlock->SetSubData(BUFFER_USAGE::UNIFORM, &uniformBlockData, sizeof(CameraUniformBlock), 0);
}

void Camera::Render()
{
	if (!GetEnable())
	{
		return;
	}

	pipeline->Render();
	//// Update Ratio
	//glm::vec2 renderTargetSize = renderTarget->GetSize();
	//ratio = renderTargetSize.x / renderTargetSize.y;


	//Scene* currentScene = Game::ActiveSceneGetPointer();
	//
	//std::list<MeshComponent*> meshComponents = ComponentManager::GetInstance()->GetMeshComponents();

	//std::list<Light*> lights = ComponentManager::GetInstance()->GetLightComponents();

	//auto cb = Game::GetCommandBuffer();

	//if (hasPostProcessing)
	//{
	//	cb->SetRenderTarget(hdrTarget);
	//}
	//else
	//{
	//	cb->SetRenderTarget(renderTarget);
	//}

	//if (renderTarget->HasDepth())
	//{
	//	cb->EnableDepth();
	//}

	//cb->SetViewport(glm::vec2(0.0f, 0.0f), glm::vec2(renderTargetSize.x, renderTargetSize.y));
	//cb->SetClearDepth(1.0f);
	//cb->SetClearColor(clearColor);	
	//cb->Clear(CLEAR_BIT::COLOR | CLEAR_BIT::DEPTH);

	//// Render Stuff
	//for (auto meshComponent : meshComponents)
	//{
	//	if (cullingMask != EVERY_THING)
	//	{
	//		if ((meshComponent->GetOwner()->GetLayerFlag() & cullingMask) == 0)
	//		{
	//			continue;
	//		}
	//	}
	//	auto materials = meshComponent->materials;
	//	
	//	Mesh* mesh = meshComponent->mesh;
	//	assert(mesh != nullptr);

	//	glm::mat4 transformation = meshComponent->GetOwner()->GetLocalToWorldMatrix();

	//	for (int i = 0; i < mesh->children.size(); i++)
	//	{
	//		if (i >= materials.size())
	//		{
	//			cb->RenderMesh(this, materials[0], mesh->children[i], transformation);
	//		}
	//		else
	//		{
	//			cb->RenderMesh(this, materials[i], mesh->children[i], transformation);
	//		}
	//	}
	//}

	//// Render skybox
	//cb->RenderSkyBox(glm::mat4(glm::mat3(GetViewMatrix())), GetProjection());

	//// Post Processing
	//if (hasPostProcessing)
	//{
	//	cb->SetRenderTarget(this->renderTarget);
	//	cb->DisableDepth();

	//	postProcessingMaterial->SetTextureProperty("hdrTarget", hdrTarget->GetAttachmentTexture(0));
	//	postProcessingMaterial->SetFloat("exposure", 1.0f);

	//	cb->RenderQuad(glm::vec2(0.0f, 0.0f), renderTargetSize, GetRenderTargetProjection(), postProcessingMaterial);
	//	cb->EnableDepth();
	//}

	//cb->Submit();
}

Ray Camera::ScreenRay(float mouseX, float mouseY)
{
	Ray result = {};
	result.origin = GetOwner()->GetPosition();

	glm::vec2 renderTargetSize = renderTarget->GetSize();

	float x = (2.0f * mouseX) / renderTargetSize.x - 1.0f;
	float y = 1.0f - (2.0f * mouseY) / renderTargetSize.y;
	float z = 1.0f;

	glm::vec3 ray_nds = glm::vec3(x, y, z);

	glm::vec4 rayClip = glm::vec4(ray_nds.x, ray_nds.y, -1.0f, 1.0f);

	glm::vec4 ray_eye = glm::inverse(GetProjection()) * rayClip;

	ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0f, 0.0f);
	glm::vec3 ray_wor = glm::normalize((glm::inverse(GetViewMatrix()) * ray_eye));

	result.direction = ray_wor;

	return result;
}
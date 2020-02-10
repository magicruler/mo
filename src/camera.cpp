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

struct CameraUniformBlock
{
	alignas(16) glm::mat4 projection;
	alignas(16) glm::mat4 view;
};

void Camera::Clear()
{
	ComponentManager::GetInstance()->AddToAvaliableCameraComponentsList(this);
}

Camera::Camera()
	:Component()
{
	// default render target
	renderTarget = Game::MainRenderTargetGetPointer();

	uniformBlock = new GPUBuffer();
	auto size = sizeof(CameraUniformBlock);
	uniformBlock->SetData(BUFFER_USAGE::UNIFORM, nullptr, size, BUFFER_DRAW_TYPE::STREAM_DRAW);
}

glm::mat4 Camera::GetProjection()
{
	return glm::perspective(fov, ratio, nearPlane, farPlane);
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
	// Write Uniform Buffer
	CameraUniformBlock uniformBlockData;
	uniformBlockData.projection = GetProjection();
	uniformBlockData.view = GetViewMatrix();
	
	uniformBlock->SetSubData(BUFFER_USAGE::UNIFORM, &uniformBlockData, sizeof(CameraUniformBlock), 0);
}

void Camera::Render()
{
	glm::vec2 renderTargetSize = renderTarget->GetSize();
	Scene* currentScene = Game::ActiveSceneGetPointer();
	ratio = renderTargetSize.x / renderTargetSize.y;

	std::list<MeshComponent*> meshComponents = ComponentManager::GetInstance()->GetMeshComponents();

	std::list<Light*> lights = ComponentManager::GetInstance()->GetLightComponents();

	auto cb = Game::GetCommandBuffer();

	cb->SetRenderTarget(renderTarget);

	if (renderTarget->HasDepth())
	{
		cb->EnableDepth();
	}

	cb->SetViewport(glm::vec2(0.0f, 0.0f), glm::vec2(renderTargetSize.x, renderTargetSize.y));
	cb->SetClearDepth(1.0f);
	cb->SetClearColor(clearColor);	
	cb->Clear(CLEAR_BIT::COLOR | CLEAR_BIT::DEPTH);

	// Render Stuff
	for (auto meshComponent : meshComponents)
	{
		auto materials = meshComponent->materials;
		
		Mesh* mesh = meshComponent->mesh;
		assert(mesh != nullptr);

		glm::mat4 transformation = meshComponent->GetOwner()->GetLocalToWorldMatrix();

		for (int i = 0; i < mesh->children.size(); i++)
		{
			if (i >= materials.size())
			{
				cb->RenderMesh(this, materials[0], mesh->children[i], transformation);
			}
			else
			{
				cb->RenderMesh(this, materials[i], mesh->children[i], transformation);
			}
		}
	}

	cb->Submit();
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
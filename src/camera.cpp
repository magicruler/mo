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

void Camera::Clear()
{
	ComponentManager::GetInstance()->AddToAvaliableCameraComponentsList(this);
}

Camera::Camera()
	:Component()
{
	// default render target
	renderTarget = Game::MainRenderTargetGetPointer();
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
	Actor* actor = this->GetParent();

	glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 worldPos = actor->GetPosition();
	// spdlog::info("Up Length Is {} ", glm::length(actor->GetUp()));
	glm::mat4 lookAtMatrix = glm::lookAt(worldPos, worldPos + actor->GetForward(), worldUp);
	
	return lookAtMatrix;
}

// TODO: TEMP POSITION FOR RENDERING CODE
void RenderMesh(Camera* camera, Material* material, Mesh* mesh, Scene* scene, glm::mat4& transformation, std::list<Light*>& lights)
{
	material->Use();
	std::vector<MaterialExtension> extensions = material->GetExtensions();
	for (auto extension : extensions)
	{
		if (extension == MaterialExtension::MODEL_VIEW_PROJECTION)
		{
			material->SetMatrix4("model", transformation);
			material->SetMatrix4("view", camera->GetViewMatrix());
			material->SetMatrix4("projection", camera->GetProjection());
		}
		else if (extension == MaterialExtension::TIME)
		{
			material->SetFloat("time", Time::GetTime());
		}
		else if (extension == MaterialExtension::AMBIENT)
		{
			material->SetVector3("ambient", scene->GetAmbient());
		}
		else if (extension == MaterialExtension::NEAREST_LIGHT)
		{
			// TODO Multiple Light, Forward Renderer
			if (lights.size() > 0)
			{
				Light* light = lights.front();
				material->SetVector3("lightColor", light->GetLightIntensityColor());
				material->SetVector3("lightPos", light->GetParent()->GetPosition());
			}
		}
		else if (extension == MaterialExtension::CAMERA)
		{
			material->SetVector3("cameraPos", camera->GetParent()->GetPosition());
		}
	}
	
	material->Use();

	mesh->vertexArray->Bind();
	glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);
	mesh->vertexArray->UnBind();
}

void Camera::Render()
{
	glm::vec2 renderTargetSize = renderTarget->GetSize();
	Scene* currentScene = Game::ActiveSceneGetPointer();
	ratio = renderTargetSize.x / renderTargetSize.y;

	std::list<MeshComponent*> meshComponents = ComponentManager::GetInstance()->GetMeshComponents();

	std::list<Light*> lights = ComponentManager::GetInstance()->GetLightComponents();

	renderTarget->Bind();
	if (renderTarget->HasDepth())
	{
		glEnable(GL_DEPTH_TEST);
	}

	glViewport(0, 0, (int)renderTargetSize.x, (int)renderTargetSize.y);
	
	glClearDepth(1.0f);
	glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
	
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	// Render Stuff
	for (auto meshComponent : meshComponents)
	{
		Material* material = meshComponent->material;
		assert(material != nullptr);

		Mesh* mesh = meshComponent->mesh;
		assert(mesh != nullptr);

		glm::mat4 transformation = meshComponent->GetParent()->GetLocalToWorldMatrix();

		RenderMesh(this, material, mesh, currentScene, transformation, lights);
	}

	renderTarget->Unbind();
}

Ray Camera::ScreenRay(float mouseX, float mouseY)
{
	Ray result = {};
	result.origin = GetParent()->GetPosition();

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
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

Camera::Camera()
	:Actor()
{
	// default render target
	renderTarget = Game::MainRenderTargetGetPointer();
	int a = 1;
}

void Camera::Tick()
{
	auto targetSize = renderTarget->GetSize();
	ratio = targetSize.x / targetSize.y;
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
	glm::mat4 modelMatrix = GetTransform();
	
	glm::mat3x3 rotationScaleMatrix = glm::mat3x3(glm::transpose(modelMatrix));
	glm::vec3 worldUp = glm::normalize(rotationScaleMatrix * glm::vec3(0.0f, 1.0f, 0.0f));
	glm::vec3 worldForward = glm::normalize(rotationScaleMatrix * glm::vec3(0.0f, 0.0f, -1.0f));
	
	glm::vec3 worldPos = GetWorldPosition();
	glm::mat4 lookAtMatrix = glm::lookAt(worldPos, worldPos + worldForward, worldUp);
	
	return lookAtMatrix;
}

// TODO: TEMP POSITION FOR RENDERING CODE
void RenderMesh(Camera* camera, Material* material, Mesh* mesh, glm::mat4& transformation)
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
	}
	
	material->Use();

	mesh->vertexArray->Bind();
	glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);
	mesh->vertexArray->UnBind();
}

void Camera::Render()
{
	glm::vec2 renderTargetSize = renderTarget->GetSize();
	std::vector<Actor*> renderables = Game::ActiveSceneGetPointer()->GetRenderables();

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
	for (auto renderableActor : renderables)
	{
		Material* material = renderableActor->GetMaterial();
		assert(material != nullptr);

		Mesh* mesh = renderableActor->GetMesh();
		assert(mesh != nullptr);

		glm::mat4 transformation = renderableActor->GetTransform();

		RenderMesh(this, material, mesh, transformation);
	}

	renderTarget->Unbind();
}


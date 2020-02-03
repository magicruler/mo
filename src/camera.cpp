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

Camera::Camera()
	:Actor()
{
	// default render target
	renderTarget = Game::MainRenderTargetGetPointer();
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

Ray Camera::CameraRay(float screenX, float screenY)
{
	Ray result = {};

	glm::vec2 screenPixelSize = renderTarget->GetSize();
	float halfFox = fov * 0.5f * (Math::PI / 180.0f);

	float screenPhysicalHeight = 1.0f * tanf(halfFox) * 2.0f;
	float screenPhysicalWidth = ratio * screenPhysicalHeight;

	float pixelPhysicalWidth = screenPhysicalWidth / screenPixelSize.x;
	float pixelPhysicalHeight = screenPhysicalHeight / screenPixelSize.y;

	// -Z Is Forward, remember!
	float targetZ = 1.0f;
	float targetX = -0.5f * screenPhysicalWidth + screenX * pixelPhysicalWidth + 0.5f * pixelPhysicalWidth;
	float targetY = 0.5f * screenPhysicalHeight - screenY * pixelPhysicalHeight - 0.5f * pixelPhysicalHeight;

	glm::mat3 model = glm::mat3(glm::transpose(GetTransform()));

	glm::vec3 worldUp = glm::normalize( glm::vec3(0.0f, 1.0f, 0.0f));
	glm::vec3 worldForward = glm::normalize(  glm::vec3(0.0f, 0.0f, -1.0f));
	glm::vec3 worldRight = glm::normalize( glm::vec3(1.0f, 0.0f, 0.0f));

	result.direction = glm::normalize(targetZ * worldForward + targetY * worldUp + targetX * worldRight);
	result.origin = this->GetWorldPosition();

	return result;
}

Ray Camera::ScreenRay(float screenX, float screenY)
{
	Ray result = {};

	glm::vec2 screenPixelSize = renderTarget->GetSize();
	float halfFox = fov * 0.5f * (Math::PI / 180.0f);

	float nearScreenPhysicalHeight = 10.0f * tanf(halfFox) * 2.0f;
	float nearScreenPhysicalWidth = ratio * nearScreenPhysicalHeight;

	float farScreenPhysicalHeight = 1.0f * tanf(halfFox) * 2.0f;
	float farScreenPhysicalWidth = ratio * farScreenPhysicalHeight;

	float nearPixelPhysicalHeight = nearScreenPhysicalHeight / screenPixelSize.y;
	float nearPixelPhysicalWidth = nearScreenPhysicalWidth / screenPixelSize.x;

	float farPixelPhysicalHeight = farScreenPhysicalHeight / screenPixelSize.y;
	float farPixelPhysicalWidth = farScreenPhysicalWidth / screenPixelSize.x;

	// -Z Is Forward, remember!
	float nearTargetZ = nearPlane;
	float nearTargetX = -0.5f * nearScreenPhysicalWidth + screenX * nearPixelPhysicalWidth + 0.5f * nearPixelPhysicalWidth;
	float nearTargetY = 0.5f * nearScreenPhysicalHeight - screenY * nearPixelPhysicalHeight - 0.5f * nearPixelPhysicalHeight;

	float farTargetZ = 10.0f;
	float farTargetX = -0.5f * farScreenPhysicalWidth + screenX * farPixelPhysicalWidth + 0.5f * farPixelPhysicalWidth;
	float farTargetY = 0.5f * farScreenPhysicalHeight - screenY * farPixelPhysicalHeight - 0.5f * farPixelPhysicalHeight;

	glm::mat3 model = glm::mat3(glm::transpose(GetTransform()));

	glm::vec3 worldUp = glm::normalize(model * glm::vec3(0.0f, 1.0f, 0.0f));
	glm::vec3 worldForward = glm::normalize(model * glm::vec3(0.0f, 0.0f, -1.0f));
	glm::vec3 worldRight = glm::normalize(model * glm::vec3(1.0f, 0.0f, 0.0f));

	glm::vec3 worldPos = GetWorldPosition();

	glm::vec3 nearPosition = worldPos + worldUp * nearTargetY + worldForward * nearTargetZ + worldRight * nearTargetX;
	glm::vec3 farPosition = worldPos + worldUp * farTargetY + worldForward * farTargetZ + worldRight * farTargetX;

	/*result.origin = nearPosition;
	result.direction = glm::normalize(farPosition - nearPosition);*/

	result.direction = glm::normalize(nearTargetZ * worldForward + nearTargetY * worldUp + nearTargetX * worldRight);
	result.origin = worldPos;

	return result;
}
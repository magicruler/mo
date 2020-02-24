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

			// Update Ratio
			ratio = resizeEvent.size.x / resizeEvent.size.y;
		}
	}
}

glm::mat4 Camera::GetProjection()
{
	return glm::perspective(glm::radians(fov), ratio, nearPlane, farPlane);
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
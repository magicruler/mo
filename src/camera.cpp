#include "camera.h"
#include "game.h"
#include "render_target.h"

Camera::Camera()
	:Actor()
{
	// default render target
	renderTarget = Game::MainRenderTargetGetPointer();
	int a = 1;
}

void Camera::Tick()
{
	auto mainTargetSize = Game::MainRenderTargetGetSize();
	ratio = mainTargetSize.x / mainTargetSize.y;

	spdlog::info("hello");
}

glm::mat4 Camera::GetProjection()
{
	return glm::perspective(fov, ratio, nearPlane, farPlane);
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::inverse(GetTransform());
}

void Camera::Render()
{
	glm::vec2 renderTargetSize = renderTarget->GetSize();
	
	renderTarget->Bind();
	if (renderTarget->HasDepth())
	{
		glEnable(GL_DEPTH_TEST);
	}
	glViewport(0, 0, (int)renderTargetSize.x, (int)renderTargetSize.y);
	glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	// Render Stuff

	renderTarget->Unbind();
}
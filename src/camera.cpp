#include "camera.h"
#include "game.h"

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
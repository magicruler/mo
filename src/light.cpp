#include "light.h"
#include "actor.h"
#include "component_manager.h"
#include "gpu_buffer.h"

struct CameraUniformBlock
{
	alignas(16) glm::mat4 projection;
	alignas(16) glm::mat4 view;
};

void Light::Clear()
{
	ComponentManager::GetInstance()->AddToAvaliableLightComponentsList(this);
}

void Light::SetCastShadow(bool val)
{
    this->castShadow = val;
	if (val)
	{
		shadowUniformBlock = new GPUBuffer();
		auto size = sizeof(CameraUniformBlock);
		shadowUniformBlock->SetData(BUFFER_USAGE::UNIFORM, nullptr, size, BUFFER_DRAW_TYPE::STREAM_DRAW);
	}
}

void Light::SetShadowUniformBlock()
{
	CameraUniformBlock uniformBlockData;
	uniformBlockData.projection = GetLightProjection();
	uniformBlockData.view = glm::inverse(GetOwner()->GetLocalToWorldMatrix());

	shadowUniformBlock->SetSubData(BUFFER_USAGE::UNIFORM, &uniformBlockData, sizeof(CameraUniformBlock), 0);
}
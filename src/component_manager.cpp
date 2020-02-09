#include "component_manager.h"
#include "mesh_component.h"
#include "camera.h"
#include "light.h"

ComponentManager* ComponentManager::instance = nullptr;

MeshComponent* ComponentManager::CreateMeshComponent(Mesh* mesh, std::vector<Material*>& materials)
{
	MeshComponent* result = nullptr;

	if (meshComponentsAvaliable.size() != 0)
	{
		result = meshComponentsAvaliable.back();
		meshComponentsAvaliable.pop_back();
	}
	else
	{
		result = new MeshComponent();
		meshComponents.push_back(result);
	}

	result->materials = materials;
	result->mesh = mesh;
	
	return result;
}

Camera* ComponentManager::CreateCameraComponent()
{
	Camera* result = nullptr;

	if (cameraComponentsAvaliable.size() != 0)
	{
		result = cameraComponentsAvaliable.back();
		cameraComponentsAvaliable.pop_back();
	}
	else
	{
		result = new Camera();
		cameraComponents.push_back(result);
	}

	return result;
}

Light* ComponentManager::CreateLightComponent()
{
	Light* result = nullptr;

	if (lightComponentsAvaliable.size() != 0)
	{
		result = lightComponentsAvaliable.back();
		lightComponentsAvaliable.pop_back();
	}
	else
	{
		result = new Light();
		lightComponents.push_back(result);
	}

	return result;
}

void ComponentManager::Init()
{
	instance = new ComponentManager();
}

void ComponentManager::Destroy()
{
	delete instance;
}

ComponentManager* ComponentManager::GetInstance()
{
	assert(instance != nullptr);
	return instance;
}

ComponentManager::ComponentManager()
{

}

ComponentManager::~ComponentManager()
{

}
#include "component_manager.h"
#include "mesh_component.h"

ComponentManager* ComponentManager::instance = nullptr;

MeshComponent* ComponentManager::CreateMeshComponent(Mesh* mesh, Material* material)
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

	result->material = material;
	result->mesh = mesh;
	
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
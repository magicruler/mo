#include "mesh_component.h"
#include "component_manager.h"
#include "actor.h"

void MeshComponent::Clear()
{
	mesh = nullptr;
	materials.clear();

	ComponentManager::GetInstance()->AddToAvaliableMeshComponentsList(this);
}

void MeshComponent::Attach()
{
}
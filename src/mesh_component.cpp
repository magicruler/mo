#include "mesh_component.h"
#include "component_manager.h"
#include "actor.h"

void MeshComponent::Clear()
{
	mesh = nullptr;
	material = nullptr;

	ComponentManager::GetInstance()->AddToAvaliableMeshComponentsList(this);
}

void MeshComponent::Attach()
{
}
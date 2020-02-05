#include "actor.h"
#include "mesh.h"
#include "material.h"
#include <glm/gtx/matrix_decompose.hpp>
#include "game.h"
#include "scene.h"
#include "imgui.h"
#include "ImGuizmo.h"
#include "component.h"
#include "mesh_component.h"

void Actor::LookAt(glm::vec3 worldPosition, glm::vec3 worldUp)
{
	auto parentWorldTransform = parent->GetTransform();

	glm::vec3 worldOriginPosition = GetTransform() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	auto newSelfWorldTransform = glm::inverse(glm::lookAt(worldOriginPosition, worldPosition, worldUp));
	
	// newSelfWorldTransform = parentWorldTransform * localTransform 
	auto localTransform = glm::inverse(parentWorldTransform) * newSelfWorldTransform;

	glm::vec3 localScale;
	glm::quat localRotation;
	glm::vec3 localTranslation;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(localTransform, localScale, localRotation, localTranslation, skew, perspective);

	SetLocalPosition(localTranslation);
	SetLocalScale(localScale);
	SetLocalRotation(glm::eulerAngles(localRotation));

	SetDirty();
}

AABB ComputeMeshAABBWithTransformation(Mesh* mesh, glm::mat4 transformation)
{
	TransformedAABB taabb = {};
	taabb.originalAABB = AABB(mesh->AABBMin, mesh->AABBMax);
	taabb.transformation = transformation;
	taabb.ComputeBounding();

	return taabb.bounding;
}

void ComputeAABB(Actor* actor)
{
	auto meshCom = actor->GetComponent<MeshComponent>();

	if (meshCom != nullptr)
	{
		auto newAABB = AABB();
		newAABB.Append(ComputeMeshAABBWithTransformation(meshCom->mesh, actor->GetTransform()));

		actor->SetAABB(newAABB);
	}
}

void Actor::UpdateTransform()
{
	if (dirty)
	{
	/*	transform = glm::translate(glm::mat4(1.0f), localPosition);
		transform = glm::eulerAngleXYZ(localRotation.x, localRotation.y, localRotation.z) * transform;
		transform = glm::scale(transform, localScale);*/
		
		ImGuizmo::RecomposeMatrixFromComponents(glm::value_ptr(localPosition), glm::value_ptr(localRotation * Math::RAD_TO_DEGREE), glm::value_ptr(localScale), glm::value_ptr(transform));

		if (parent != nullptr)
		{
			transform = parent->GetTransform() * transform;
		}
	}

	dirty = false;
	// Now We Have New World Transform
	ComputeAABB(this);
}

AABB Actor::GetAABB()
{
	return aabb;
}

void Actor::AddChild(Actor* child)
{
	child->parent = this;
	children.push_back(child);

	MarkChildrenDirty();
}

void Actor::MarkChildrenDirty()
{
	std::vector<Actor*> sceneFrontLayer;
	std::vector<Actor*> sceneBackLayer;

	sceneFrontLayer = this->GetChildren();

	while (sceneFrontLayer.size() != 0)
	{
		sceneBackLayer.clear();

		for (int i = 0; i < sceneFrontLayer.size(); i++)
		{
			auto actor = sceneFrontLayer[i];
			actor->dirty = true;
		}

		sceneFrontLayer.swap(sceneBackLayer);
	}
}

void Actor::UpdateLocalSpace()
{
	auto parentToWorldTransform = parent->GetTransform();
	
	// Parent To World * Local To Parent = New Local To World

	// Local To Parent = Inverse(Parent To World) * New Local To World

	auto localToParent = glm::inverse(parentToWorldTransform) * transform;
	
	glm::vec3 localS;
	glm::vec3 localRot;
	glm::vec3 localTranslation;
	
	ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(localToParent), glm::value_ptr(localTranslation), glm::value_ptr(localRot), glm::value_ptr(localS));
	
	SetLocalPosition(localTranslation);
	SetLocalScale(localS);
	SetLocalRotation(localRot * Math::DEGREE_TO_RAD);

	//glm::mat4 oldTransform = transform;
	//UpdateTransform();
	//glm::mat4 newTransform = transform;

	//int a = 1;
}

void Actor::AddComponent(Component* com)
{
	assert(com != nullptr);
	assert(com->GetParent() != this);

	if (com->GetParent() != nullptr)
	{
		com->GetParent()->RemoveComponent(com);
	}

	components.push_back(com);
	com->SetParent(this);
}

void Actor::RemoveComponent(Component* com)
{
	assert(com->GetParent() == this);

	for (int i = 0; i < components.size(); i++)
	{
		if (components[i] == com)
		{
			com->SetParent(nullptr);
			com->Clear();

			components.erase(components.begin() + i);
			return;
		}
	}
	assert(false);
}

Component* Actor::GetComponent(size_t hashId)
{
	for (auto com : components)
	{
		if (com->GetHashID() == hashId)
		{
			return com;
		}
	}

	return nullptr;
}
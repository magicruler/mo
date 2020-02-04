#include "actor.h"
#include "mesh.h"
#include "material.h"
#include <glm/gtx/matrix_decompose.hpp>
#include "game.h"
#include "scene.h"

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
	auto mesh = actor->GetMesh();
	if (mesh != nullptr)
	{
		auto newAABB = AABB();
		newAABB.Append(ComputeMeshAABBWithTransformation(mesh, actor->GetTransform()));

		actor->SetAABB(newAABB);
	}
}

void Actor::UpdateTransform()
{
	if (dirty)
	{
		transform = glm::translate(glm::mat4(1.0f), localPosition);
		transform = glm::eulerAngleXYZ(localRotation.x, localRotation.y, localRotation.z) * transform;
		transform = glm::scale(transform, localScale);
		
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

	glm::vec3 localScale;
	glm::quat localRot;
	glm::vec3 localTranslation;
	glm::vec3 skew;
	glm::vec4 perspective;

	glm::decompose(localToParent, localScale, localRot, localTranslation, skew, perspective);

	spdlog::info("old translation is {} {} {}", localPosition.x, localPosition.y, localPosition.z);
	spdlog::info("new  translation is {} {} {}", localTranslation.x, localTranslation.y, localTranslation.z);

	spdlog::info("old rotation is {} {} {}", localRotation.x, localRotation.y, localRotation.z);
	spdlog::info("new  rotation is {} {} {}", glm::eulerAngles(localRot).x, glm::eulerAngles(localRot).y, glm::eulerAngles(localRot).z);

	SetLocalPosition(localTranslation);
	SetLocalScale(localScale);
	SetLocalRotation(glm::eulerAngles(localRot));

	//glm::mat4 oldTransform = transform;
	//UpdateTransform();
	//glm::mat4 newTransform = transform;

	//int a = 1;
}

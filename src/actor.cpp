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
		transform = glm::translate(glm::mat4(1.0f) ,localPosition);
		transform = glm::scale(transform, localScale);
		transform = glm::eulerAngleXYZ(localRotation.x, localRotation.y, localRotation.z) * transform;

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

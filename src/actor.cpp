#include "actor.h"
#include "mesh.h"

void Actor::UpdateTransform()
{
	if (dirty)
	{
		transform = glm::translate(glm::mat4(1.0f) ,localPosition);
		transform = glm::scale(transform, localScale);
		transform = glm::eulerAngleXYZ(localRotation.x, localRotation.y, localRotation.z) * transform;

		if (parent != nullptr)
		{
			transform = parent->transform * transform;
		}
	}

	for (int i = 0; i < children.size(); ++i)
	{
		if (dirty)
		{
			children[i]->dirty = true;
		}
		children[i]->UpdateTransform();
	}

	dirty = false;
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
		actor->aabb.Append(ComputeMeshAABBWithTransformation(mesh, actor->GetTransform()));
	}

	for (auto child : actor->GetChildren())
	{
		ComputeAABB(child);
		actor->aabb.Append(child->aabb);
	}
}

void Actor::AddChild(Actor* child)
{
	child->parent = this;
	children.push_back(child);

	dirty = true;
	UpdateTransform();

	// Now all transform valid
	ComputeAABB(this);
}



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

AABB ComputeMeshAABBWithTransformation(Mesh* mesh, glm::mat4 transformation)
{
	TransformedAABB taabb = {};
	taabb.originalAABB = AABB(mesh->AABBMin, mesh->AABBMax);
	taabb.transformation = transformation;
	taabb.ComputeBounding();

	return taabb.bounding;
}

glm::mat4 Actor::GetParentTransformMatrix() const
{
	return HasParent() ? GetParent()->GetLocalToWorldMatrix() : glm::mat4(1.0f);
}

void Actor::UpdateTransform()
{
	glm::vec3 eulerAngles = glm::eulerAngles(m_localRotation) * Math::RAD_TO_DEGREE;
	ImGuizmo::RecomposeMatrixFromComponents(glm::value_ptr(m_localPosition), glm::value_ptr(eulerAngles), glm::value_ptr(m_localScale), glm::value_ptr(m_localToParentMatrix));
	
	if (!HasParent())
	{
		m_localToWorldMatrix = m_localToParentMatrix;
	}
	else
	{
		m_localToWorldMatrix = GetParentTransformMatrix() * m_localToParentMatrix;
	}
	
	for (auto child: children)
	{
		child->UpdateTransform();
	}
}
//Position===========================================================================
void Actor::SetPosition(const glm::vec3& position)
{
	if (GetPosition() == position)
	{
		return;
	}

	SetPositionLocal(!HasParent() ? position : glm::inverse(GetParent()->GetLocalToWorldMatrix()) * glm::vec4(position, 1.0f));
}
void Actor::SetPositionLocal(const glm::vec3& position)
{
	if (m_localPosition == position)
	{
		return;
	}

	m_localPosition = position;
	UpdateTransform();
}
//Rotation===========================================================================
void Actor::SetRotation(const glm::quat& rotation)
{
	if (GetRotation() == rotation)
	{
		return;
	}

	SetRotationLocal(!HasParent() ? rotation : rotation * glm::inverse(GetParent()->GetRotation()));
}

// In Degree
void Actor::SetRotationEuler(const glm::vec3& rotation)
{
	SetRotation(glm::quat_cast(glm::orientate3(rotation * Math::DEGREE_TO_RAD)));
}

void Actor::SetRotationLocal(const glm::quat& rotation)
{
	if (m_localRotation == rotation)
	{
		return;
	}

	m_localRotation = rotation;
	UpdateTransform();
}
// In Degree
void Actor::SetRotationEulerLocal(const glm::vec3& rotation)
{
	SetRotationLocal(glm::quat_cast(glm::orientate3(rotation * Math::DEGREE_TO_RAD)));
}
//Scale==============================================================================
void Actor::SetScale(const glm::vec3& scale)
{
	if (GetScale() == scale)
	{
		return;
	}

	SetScaleLocal(!HasParent() ? scale : scale / GetParent()->GetScale());
}
void Actor::SetScaleLocal(const glm::vec3& scale)
{
	if (m_localScale == scale)
	{
		return;
	}

	m_localScale = scale;

	m_localScale.x = (m_localScale.x == 0.0f) ? Math::EPSILON : m_localScale.x;
	m_localScale.y = (m_localScale.y == 0.0f) ? Math::EPSILON : m_localScale.y;
	m_localScale.z = (m_localScale.z == 0.0f) ? Math::EPSILON : m_localScale.z;

	UpdateTransform();
}
//Translation And Rotation===========================================================
void Actor::Translate(const glm::vec3& delta)
{
	if (!HasParent())
	{
		SetPositionLocal(m_localPosition + delta);
	}
	else
	{
		SetPositionLocal(m_localPosition + glm::vec3(glm::inverse(GetParent()->GetLocalToWorldMatrix()) * glm::vec4(delta, 1.0f)));
	}
}
void Actor::Rotate(const glm::vec3& delta)
{
	Rotate(glm::quat_cast(glm::orientate3(delta * Math::DEGREE_TO_RAD)));
}
void Actor::Rotate(const glm::quat& delta)
{
	if (!HasParent())
	{
		SetRotationLocal(delta);
	}
	else
	{
		SetRotationLocal(m_localRotation * glm::inverse(GetRotation()) * delta * GetRotation());
	}
}
// Directions============================================================================
glm::vec3 Actor::GetUp() const
{
	return GetRotationLocal() * glm::vec3(0.0f, 1.0f, 0.0f);
}
glm::vec3 Actor::GetForward() const
{
	return GetRotationLocal() * glm::vec3(0.0f, 0.0f, -1.0f);
}
glm::vec3 Actor::GetRight() const
{
	return GetRotationLocal() * glm::vec3(1.0f, 0.0f, 0.0f);
}
//===================================================================================
AABB Actor::GetAABB()
{
	auto meshCom = GetComponent<MeshComponent>();
	
	if (meshCom)
	{
		return ComputeMeshAABBWithTransformation(meshCom->mesh, this->GetLocalToWorldMatrix());
	}

	return AABB();
}

void Actor::SetParent(Actor* newParent)
{
	if (GetParent() == newParent)
	{
		return;
	}

	parent = newParent;
	UpdateTransform();
}

void Actor::AddChild(Actor* child)
{
	children.push_back(child);
	child->SetParent(this);
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
	com->Attach();
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
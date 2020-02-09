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
#include "ImGuizmo.h"

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
	ImGuizmo::RecomposeMatrixFromComponents(glm::value_ptr(m_localPosition), glm::value_ptr(m_localRotation), glm::value_ptr(m_localScale), glm::value_ptr(m_localToParentMatrix));

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
glm::vec3 Actor::GetRotation() const
{
	glm::vec3 rotation;
	ImGuizmo::DecomposeRotation(glm::value_ptr(m_localToWorldMatrix), glm::value_ptr(rotation));
	
	return rotation;
}

void Actor::SetRotation(const glm::vec3& rotation)
{
	if (GetRotation() == rotation)
	{
		return;
	}
	
	auto parentRotation = GetParent()->GetRotation();
	glm::mat4 parentRotationMatrix;
	ImGuizmo::RecomposeMatrixFromComponents(glm::value_ptr(glm::vec3(0.0f, 0.0f, 0.0f)), glm::value_ptr(parentRotation), glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)), glm::value_ptr(parentRotationMatrix));

	glm::mat4 newRotationMatrix;
	ImGuizmo::RecomposeMatrixFromComponents(glm::value_ptr(glm::vec3(0.0f, 0.0f, 0.0f)), glm::value_ptr(rotation), glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)), glm::value_ptr(newRotationMatrix));

	// local *  parent rotation =  new rotation mat
	glm::vec3 newLocalRotation;
	ImGuizmo::DecomposeRotation(glm::value_ptr(newRotationMatrix * glm::inverse(parentRotationMatrix)), glm::value_ptr(newLocalRotation));

	SetRotationLocal(!HasParent() ? rotation : newLocalRotation);
}

void Actor::SetRotationLocal(const glm::vec3& rotation)
{
	if (m_localRotation == rotation)
	{
		return;
	}

	m_localRotation = rotation;
	UpdateTransform();
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

void Actor::TranslateLocal(const glm::vec3& delta)
{
	SetPositionLocal(m_localPosition + delta);
}

void Actor::Rotate(const glm::vec3& delta)
{
	if (!HasParent())
	{
		SetRotationLocal(m_localRotation + delta);
	}
	else
	{
		SetRotation(GetRotation() + delta);
	}
}

// Directions============================================================================
glm::vec3 Actor::GetUpLocal() const
{
	auto rotation = GetRotationLocal() * Math::DEGREE_TO_RAD;
	return  glm::eulerAngleXYZ(rotation.x, rotation.y, rotation.z) * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
}
glm::vec3 Actor::GetUp() const
{
	auto rotation = GetRotation() * Math::DEGREE_TO_RAD;
	return  glm::eulerAngleXYZ(rotation.x, rotation.y, rotation.z) * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
}
glm::vec3 Actor::GetForwardLocal() const
{
	auto rotation = GetRotationLocal() * Math::DEGREE_TO_RAD;
	return glm::eulerAngleXYZ(rotation.x, rotation.y, rotation.z) * glm::vec4(0.0f, 0.0f, -1.0f, 1.0f);
}

glm::vec3 Actor::GetForward() const
{
	auto rotation = GetRotation() * Math::DEGREE_TO_RAD;
	return glm::eulerAngleXYZ(rotation.x, rotation.y, rotation.z) * glm::vec4(0.0f, 0.0f, -1.0f, 1.0f);
}
glm::vec3 Actor::GetRightLocal() const
{
	auto rotation = GetRotationLocal() * Math::DEGREE_TO_RAD;
	return glm::eulerAngleXYZ(rotation.x, rotation.y, rotation.z) * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
}
glm::vec3 Actor::GetRight() const
{
	auto rotation = GetRotation() * Math::DEGREE_TO_RAD;
	return glm::eulerAngleXYZ(rotation.x, rotation.y, rotation.z) * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
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
	assert(com->GetOwner() != this);

	if (com->GetOwner() != nullptr)
	{
		com->GetOwner()->RemoveComponent(com);
	}

	components.push_back(com);
	com->SetParent(this);
	com->Attach();
}

void Actor::RemoveComponent(Component* com)
{
	assert(com->GetOwner() == this);

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
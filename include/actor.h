#pragma once
#include "common.h"
#include "aabb.h"

class Component;

enum LAYER_MASK
{
	GENERAL = 1 << 0,
	ONLY_FOR_EDITOR_OBJECTS = 1 << 1,
};

enum PROPERTY_MASK
{
	NOTHING = 1 << 0,
	NON_SERIALIZED = 1 << 1,
};

enum SPACE
{
	LOCAL = 0,
	WORLD = 1,
};

class Actor : Object
{
	MO_OBJECT("Actor")
public:
	friend class EditorInspector;
	friend class EditorSceneView;

	Actor()
	{
		parent = nullptr;

		m_localPosition = glm::vec3(0.0f, 0.0f, 0.0f);
		m_localRotation = glm::vec3(0.0f, 0.0f, 0.0f);
		m_localScale = glm::vec3(1.0f, 1.0f, 1.0f);

		m_localToParentMatrix = glm::mat4(1.0f);
		m_localToWorldMatrix = glm::mat4(1.0f);
	}
	//Matrix==============================================================================
	const glm::mat4& GetLocalToWorldMatrix() const
	{
		return m_localToWorldMatrix;
	}

	const glm::mat4& GetLocalToParentMatrix() const
	{
		return m_localToParentMatrix;
	}

	void UpdateTransform();
	// Position==============================================================================
	glm::vec3 GetPosition() { return Math::DecomposeTranslation(m_localToWorldMatrix); }
	const glm::vec3& GetPositionLocal() { return m_localPosition; }
	void SetPosition(const glm::vec3& position);
	void SetPositionLocal(const glm::vec3& position);
	// Rotation==============================================================================
	glm::vec3 GetRotation() const;
	const glm::vec3& GetRotationLocal() const { return m_localRotation; }
	void SetRotation(const glm::vec3& rotation);
	void SetRotationLocal(const glm::vec3& rotation);
	// Scale=================================================================================
	glm::vec3 GetScale() { return Math::DecomposeScale(m_localToWorldMatrix); }
	const glm::vec3& GetScaleLocal() const { return m_localScale; }
	void SetScale(const glm::vec3& scale);
	void SetScaleLocal(const glm::vec3& scale);
	// Translation And Rotation==============================================================
	void Translate(const glm::vec3& delta);
	void TranslateLocal(const glm::vec3& delta);
	void Rotate(const glm::vec3& delta);
	// Directions============================================================================
	glm::vec3 GetUp() const;
	glm::vec3 GetUpLocal() const;
	glm::vec3 GetForward() const;
	glm::vec3 GetForwardLocal() const;
	glm::vec3 GetRight() const;
	glm::vec3 GetRightLocal() const;
	//=======================================================================================
	virtual void Tick()
	{

	}

	inline Actor* HasParent() const
	{
		return parent;
	}

	inline Actor* GetParent() const
	{
		return parent;
	}

	inline std::vector<Actor*> GetChildren()
	{
		return children;
	}

	void AddChild(Actor* child);
	void SetParent(Actor* newParent);

	std::string GetName()
	{
		return name;
	}

	void SetName(const std::string newName)
	{
		name = newName;
	}

	unsigned int GetLayerFlag()
	{
		return layerFlag;
	}

	void SetLayerFlag(unsigned int flag)
	{
		layerFlag = flag;
	}

	unsigned int GetPropertyFlag()
	{
		return propertyFlag;
	}

	void SetPropertyFlag(unsigned int flag)
	{
		propertyFlag = flag;
	}

	AABB GetAABB();

	void AddComponent(Component* com);
	void RemoveComponent(Component* com);

	template<typename T>
	T* GetComponent()
	{
		size_t hashId = T::GetHashIDStatic();
		return (T*)GetComponent(hashId);
	}

	Component* GetComponent(size_t hashId);

private:

	glm::mat4 GetParentTransformMatrix() const;

	glm::vec3 m_localRotation;
	glm::vec3 m_localPosition;
	glm::vec3 m_localScale;

	glm::mat4 m_localToWorldMatrix;
	glm::mat4 m_localToParentMatrix;

	Actor* parent;
	std::vector<Actor*> children;

	std::string name = "";

	unsigned int layerFlag = LAYER_MASK::GENERAL;
	unsigned int propertyFlag = PROPERTY_MASK::NOTHING;

	std::vector<Component*> components;
};
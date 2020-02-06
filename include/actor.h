#pragma once
#include "common.h"
#include "aabb.h"

class Mesh;
class Material;
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
		mesh = nullptr;
        material = nullptr;

		localToWorldMatrix = glm::mat4(1.0f);
		
		localPosition = glm::vec3(0.0f);
		localRotation = glm::vec3(0.0f);
		localScale = glm::vec3(1.0f, 1.0f, 1.0f);
	}

	virtual void Tick()
	{

	}

	inline Actor* GetParent()
	{
		return parent;
	}

	inline std::vector<Actor*> GetChildren()
	{
		return children;
	}

	void AddChild(Actor* child);

	void CollectSubTreeInternal(std::vector<Actor*>& vec, Actor* actor)
	{
		vec.push_back(actor);

		for (auto child : actor->children)
		{
			CollectSubTreeInternal(vec, child);
		}
	}

	std::vector<Actor*> CollectSubTree()
	{
		std::vector<Actor*> result;
		CollectSubTreeInternal(result, this);
		return result;
	}

	inline void Translate(glm::vec3 value)
	{
		SetLocalPosition(localPosition + value);
	}

	inline void Rotate(glm::vec3 value)
	{
		SetLocalRotation(localRotation + value);
	}

	inline void SetLocalPosition(glm::vec3 pos)
	{
		localPosition = pos;
		SetDirty();
	}

	inline glm::vec3 GetLocalPosition()
	{
		return localPosition;
	}

	void ComputeAABB();

	// TODO, USE CACHED POSITION
	glm::vec3 GetWorldPosition()
	{
		return GetLocalToWorldMatrix() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}

	inline void SetLocalScale(glm::vec3 scale)
	{
		localScale = scale;
		SetDirty();
	}

	inline void SetLocalRotation(glm::vec3 rotation)
	{
		localRotation = rotation;
		SetDirty();
	}

	/*
	target, world position
	*/
	void LookAt(glm::vec3 worldPosition, glm::vec3 worldUp);

	glm::mat4 GetLocalToWorldMatrix()
	{
		if (dirty)
		{
			UpdateTransform();
		}

		return localToWorldMatrix;
	}

	void UpdateTransform();

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

	AABB GetAABBWithoutPromise()
	{
		return aabb;
	}

	void SetAABB(AABB& value)
	{
		aabb = value;
	}

	void SetDirty()
	{
		if (!dirty)
		{
			dirty = true;
			MarkChildrenDirty();
		}
	}

	void MarkChildrenDirty();

	void UpdateLocalSpace();

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

	AABB aabb;
	Mesh* mesh;
	Material* material;

	Actor* parent;
	std::vector<Actor*> children;
	glm::mat4 localToWorldMatrix;
	
	bool dirty = false;

	glm::vec3 localPosition;
	glm::vec3 localRotation;
	glm::vec3 localScale;

	// world forward
	glm::vec3 forward;
	// world up
	glm::vec3 up;
	// world right
	glm::vec3 right;

	std::string name = "";

	unsigned int layerFlag = LAYER_MASK::GENERAL;
	unsigned int propertyFlag = PROPERTY_MASK::NOTHING;

	std::vector<Component*> components;
};
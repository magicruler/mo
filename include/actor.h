#pragma once
#include "common.h"
#include "aabb.h"

class Mesh;
class Material;

class Actor : Object
{
	MO_OBJECT("Actor")
public:
	
	Actor()
	{
		parent = nullptr;
		mesh = nullptr;
        material = nullptr;

		transform = glm::mat4(1.0f);
		
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

	void SetMesh(Mesh* mesh)
	{
		this->mesh = mesh;
	}

	Mesh* GetMesh()
	{
		return mesh;
	}

    void SetMaterial(Material* material)
	{
		this->material = material;
	}

	Material* GetMaterial()
	{
		return material;
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
		dirty = true;
	}

	// TODO, USE CACHED POSITION
	glm::vec3 GetWorldPosition()
	{
		return GetTransform() * glm::vec4(localPosition.x, localPosition.y, localPosition.z, 1.0f);
	}

	inline void SetLocalScale(glm::vec3 scale)
	{
		localScale = scale;
		dirty = true;
	}

	inline void SetLocalRotation(glm::vec3 rotation)
	{
		localRotation = rotation;
		dirty = true;
	}

	/*
	target, world position
	*/
	void LookAt(glm::vec3 worldPosition, glm::vec3 worldUp);

	glm::mat4 GetTransform()
	{
		if (dirty)
		{
			UpdateTransform();
		}

		return transform;
	}

	void UpdateTransform();

private:
	Mesh* mesh;
	Material* material;

	Actor* parent;
	std::vector<Actor*> children;
	glm::mat4 transform;
	
	bool dirty = false;

	glm::vec3 localPosition;
	glm::vec3 localRotation;
	glm::vec3 localScale;

	bool tickable = false;
	std::string name = "";

public:
	std::string GetName()
	{
		return name;
	}

	void SetName(const std::string newName)
	{
		name = newName;
	}

	AABB aabb;
};
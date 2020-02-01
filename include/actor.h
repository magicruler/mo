#pragma once
#include "common.h"
#include "aabb.h"

class Mesh;

class Actor
{
public:
	
	Actor()
	{
		parent = nullptr;
		mesh = nullptr;

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

	inline void SetLocalPosition(glm::vec3 pos)
	{
		localPosition = pos;
		dirty = true;
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
	
	Actor* parent;
	std::vector<Actor*> children;
	glm::mat4 transform;
	
	bool dirty = false;

	glm::vec3 localPosition;
	glm::vec3 localRotation;
	glm::vec3 localScale;

public:
	AABB aabb;
};
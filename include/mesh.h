#pragma once
#include "common.h"

struct MeshVertex
{
	MeshVertex()
	{
		position = glm::vec3(0.0f);
		normal = glm::vec3(0.0f);
		uv = glm::vec2(0.0f);
		tangent = glm::vec3(0.0f);
		bitangent = glm::vec3(0.0f);
	}

	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
	glm::vec3 tangent;
	glm::vec3 bitangent;
};

class VertexArray;
class GPUBuffer;

class SubMesh
{
public:
	std::string name;
	std::vector<MeshVertex> vertices;
	std::vector<unsigned int> indices;

	glm::vec3 AABBMin;
	glm::vec3 AABBMax;

	void CreateGPUResource();
	VertexArray* vertexArray = nullptr;
	GPUBuffer* elementBuffer = nullptr;
	GPUBuffer* arrayBuffer = nullptr;
};

class Mesh
{
public:
	std::string name;

	glm::vec3 AABBMin;
	glm::vec3 AABBMax;

	void CreateGPUResource();
	std::vector<SubMesh*> children;
};
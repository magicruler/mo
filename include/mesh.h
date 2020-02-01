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

class Mesh
{
public:
	std::string name;
	std::vector<MeshVertex> vertices;
	std::vector<uint16_t> indices;

	glm::vec3 AABBMin;
	glm::vec3 AABBMax;
};
#pragma once
#include "common.h"

class Texture;
class Shader;
class Material;
class Mesh;

/*
Lazy Initialization, Lazy Management XD
*/
namespace Resources
{
	Texture* GetTexture(const std::string& path);
	Texture* LoadTexture(const std::string& path);

	Shader* GetShader(const std::string& path);
	Shader* LoadShader(const std::string& path);

	Material* GetMaterial(const std::string& path);
	Material* LoadMaterial(const std::string& path);

	Mesh* GetMesh(const std::string& path);
	Mesh* LoadMesh(const std::string& path);
};
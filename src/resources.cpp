#include "resources.h"

#include "shader.h"
#include "texture.h"
#include "material.h"

#include "string_utils.h"
#include "serialization.h"

namespace Resources
{
	/*
	Store All Loaded Textures
	*/
	std::map<std::string, Texture*> textures;
	/*
	Store All Loaded Materials
	*/
	std::map<std::string, Material*> materials;
	/*
	Store All Loaded Shaders
	*/
	std::map<std::string, Shader*> shaders;

	/*
	Try Get Texture, If Not Exist, Load It
	*/
	Texture* GetTexture(const std::string& path)
	{
		auto it = textures.find(path);
		// Don't Find, Load
		if (it == textures.cend())
		{
			return LoadTexture(path);
		}
		// Find, Return
		else
		{
			return it->second;
		}
	}

	Texture* LoadTexture(const std::string& path)
	{
		auto metaContent = StringUtils::ReadFile("textures/" + path);
		Texture* texture = Serialization::DeserializeTexture(metaContent);
		
		textures[path] = texture;
		
		return texture;
	}

	Shader* GetShader(const std::string& path)
	{
		auto it = shaders.find(path);
		// Don't Find, Load
		if (it == shaders.cend())
		{
			return LoadShader(path);
		}
		// Find, Return
		else
		{
			return it->second;
		}
	}

	Shader* LoadShader(const std::string& path)
	{
		auto metaContent = StringUtils::ReadFile("shaders/" + path);
		Shader* shader = Serialization::DeserializeShader(metaContent);

		shaders[path] = shader;

		return shader;
	}

	Material* GetMaterial(const std::string& path)
	{
		auto it = materials.find(path);
		// Don't Find, Load
		if (it == materials.cend())
		{
			return LoadMaterial(path);
		}
		// Find, Return
		else
		{
			return it->second;
		}
	}

	Material* LoadMaterial(const std::string& path)
	{
		auto metaContent = StringUtils::ReadFile("materials/" + path);
		Material* material = Serialization::DeserializeMaterial(metaContent);

		materials[path] = material;

		return material;
	}
};
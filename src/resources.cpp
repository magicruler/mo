#include "resources.h"

#include "shader.h"
#include "texture.h"
#include "material.h"
#include "mesh.h"
#include "render_target.h"

#include "string_utils.h"
#include "serialization.h"

#include <filesystem>
#include "game.h"

namespace Resources
{
	/*
	Store All Resource Type Render Targets
	*/
	std::map<std::string, RenderTarget*> renderTargets;

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
	Store All Loaded Meshes
	*/
	std::map<std::string, Mesh*> meshes;

	//// Initialize All Render Targets In assets/render-targets
	//void InitRenderTargets()
	//{
	//	const std::filesystem::path assetPath(Game::GetEnviromentPath());
	//	for (auto& p : std::filesystem::recursive_directory_iterator("render-targets"))
	//	{
	//		spdlog::info("{}", p.path().string());
	//		auto content = StringUtils::ReadFile(p.path().string());
	//		renderTargets[p.path().string()] = Serialization::DeserializeRenderTarget(content);
	//	}
	//}

	RenderTarget* GetRenderTarget(const std::string& path)
	{
		auto it = renderTargets.find(path);
		// Don't Find, Load
		if (it == renderTargets.cend())
		{
			return LoadRenderTarget(path);
		}
		// Find, Return
		else
		{
			return it->second;
		}
	}

	RenderTarget* LoadRenderTarget(const std::string& path)
	{
		auto metaContent = StringUtils::ReadFile("render-targets/" + path);
		RenderTarget* renderTarget = Serialization::DeserializeRenderTarget(metaContent);

		renderTargets[path] = renderTarget;

		return renderTarget;
	}

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

	Mesh* GetMesh(const std::string& path)
	{
		auto it = meshes.find(path);
		// Don't Find, Load
		if (it == meshes.cend())
		{
			return LoadMesh(path);
		}
		// Find, Return
		else
		{
			return it->second;
		}
	}

	Mesh* LoadMesh(const std::string& path)
	{
		auto metaContent = StringUtils::ReadFile("meshes/" + path);
		Mesh* mesh = Serialization::DeserializeMesh(metaContent);

		meshes[path] = mesh;

		return mesh;
	}
};
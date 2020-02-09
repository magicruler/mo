#include "serialization.h"
#include "configs.h"
#include "json.hpp"
#include "string_utils.h"
#include "scene.h"
#include "actor.h"
#include "camera.h"
#include "light.h"

#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "shader.h"
#include "material.h"
#include "mesh.h"

#include "resources.h"

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#ifndef __glad_h_
#include <glad/glad.h>
#endif

#include "component_manager.h"
#include "mesh_component.h"

using json = nlohmann::json;

namespace Serialization
{
	void LoadProject()
	{
		auto projectContent = StringUtils::ReadFile("project.json");
		auto projectJsonObject = json::parse(projectContent);
		
		Configuration::SetFPS(projectJsonObject["frameRate"].get<float>());
		Configuration::SetEntryScene(projectJsonObject["entryScene"].get<std::string>());
	}

	glm::vec4 DeserilizeVector4(json& jObject)
	{
		assert(jObject.is_array());
		assert(jObject.size() == 4);
		return glm::vec4(jObject[0], jObject[1], jObject[2], jObject[3]);
	}

	glm::vec3 DeserilizeVector3(json& jObject)
	{
		assert(jObject.is_array());
		assert(jObject.size() == 3);
		return glm::vec3(jObject[0], jObject[1], jObject[2]);
	}

	void ProcessTransformation(json& transformObject, Actor* actor)
	{
		actor->SetPositionLocal(DeserilizeVector3(transformObject["position"]));
		actor->SetScaleLocal(DeserilizeVector3(transformObject["scale"]));
		actor->SetRotationLocal(DeserilizeVector3(transformObject["rotation"]));
	}

	void ProcessNode(json& node, Actor* currentActor)
	{
		auto typeName = node["type"].get<std::string>();
		
		if (typeName == "actor")
		{
			Actor* newActor = new Actor();
			auto name = node["name"].get<std::string>();
			newActor->SetName(name);
			
			if (node.contains("components"))
			{
				auto componentsObject = node["components"];
				assert(componentsObject.is_array());

				for (auto& comObject : componentsObject)
				{
					std::string typeName = comObject["type"].get<std::string>();
					if (typeName == "meshComponent")
					{
						auto materialPaths = comObject["materials"];
						assert(materialPaths.is_array());
						std::vector<Material*> materials;
						for (const auto& materialPathObj : materialPaths)
						{
							auto materialPath = materialPathObj.get<std::string>();
							Material* material = Resources::GetMaterial(materialPath);
							materials.push_back(material);
						}
								
						auto meshPath = comObject["mesh"].get<std::string>();
						Mesh* mesh = Resources::GetMesh(meshPath);

						MeshComponent* meshCom = ComponentManager::GetInstance()->CreateMeshComponent(mesh, materials);
						newActor->AddComponent(meshCom);
					}
					else if (typeName == "camera")
					{
						Camera* cameraCom = ComponentManager::GetInstance()->CreateCameraComponent();
	
						newActor->AddComponent(cameraCom);

						cameraCom->fov = comObject["fov"];
						cameraCom->nearPlane = comObject["nearPlane"];
						cameraCom->farPlane = comObject["farPlane"];
						cameraCom->clearColor = DeserilizeVector4(comObject["clearColor"]);
					}
					else if (typeName == "light")
					{
						Light* lightCom = ComponentManager::GetInstance()->CreateLightComponent();

						newActor->AddComponent(lightCom);
						// Light Properties
						lightCom->SetLightIntensity(comObject["lightIntensity"]);
						lightCom->SetLightColor(DeserilizeVector3(comObject["lightColor"]));
					}
				}
			}

			auto transformObject = node["transform"];
			ProcessTransformation(transformObject, newActor);
			
			currentActor->AddChild(newActor);

			auto childrenNode = node["children"];
			for (auto& childNode : childrenNode)
			{
				ProcessNode(childNode, newActor);
			}
		}
	}

	Scene* DeserializeScene(const std::string& content)
	{
		auto sceneJsonObject = json::parse(content);
		Scene* scene = new Scene();
		
		auto children = sceneJsonObject["children"];
		assert(children.is_array());
		for (auto& node : children)
		{
			ProcessNode(node, scene->rootNode);
		}

		auto ambient = sceneJsonObject["amibent"];
		assert(ambient.is_array());
		
		scene->SetAmbient(glm::vec3(ambient[0], ambient[1], ambient[2]));

		return scene;
	}

	Texture* DeserializeTexture(const std::string& content)
	{
		auto textureJsonObject = json::parse(content);
		Texture* texture = new Texture();

		std::string textureType = textureJsonObject["type"].get<std::string>();
		
		if (textureType == "texture2d")
		{
			std::string path = "textures/" + textureJsonObject["path"].get<std::string>();

			bool mipmapped = textureJsonObject["mipmap"].get<bool>();
			texture->isMipmapped = mipmapped;

			int textureWidth, textureHeight, textureChannels;
			stbi_uc* pixels = stbi_load(path.c_str(), &textureWidth, &textureHeight, &textureChannels, STBI_rgb_alpha);

			texture->SetData2D(textureWidth, textureHeight, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
			stbi_image_free(pixels);
		}

		return texture;
	}

	/* 
	Materials Own References Of Shaders, So This Function Will Try To Get Shaders From Resources 
	*/
	Material* DeserializeMaterial(const std::string& content)
	{
		auto materialJsonObject = json::parse(content);
		
		auto name = materialJsonObject["name"].get<std::string>();
		auto shaderPath = materialJsonObject["shader"].get<std::string>();
		Shader* shader = Resources::GetShader(shaderPath);
		Material* material = new Material(shader);

		material->SetName(name);

		auto properties = materialJsonObject["properties"];
		assert(properties.is_array());
		for (auto& prop : properties)
		{
			assert(prop.is_object());
			auto propName = prop["name"].get<std::string>();
			auto propType = prop["type"].get<std::string>();
			if (propType == "sampler2D")
			{
				auto texturePath = prop["value"].get<std::string>();
				Texture* texture = Resources::LoadTexture(texturePath);
				material->SetTextureProperty(propName, texture);
			}
			else if (propType == "int")
			{
				auto value = prop["value"].get<int>();
				material->SetInt(propName, value);
			}
			else if (propType == "float")
			{
				auto value = prop["value"].get<float>();
				material->SetFloat(propName, value);
			}
			else if (propType == "vec3")
			{
				auto value = DeserilizeVector3(prop["value"]);
				material->SetVector3(propName, value);
			}
		}

		auto extensions = materialJsonObject["extensions"];
		assert(extensions.is_array());
		for (auto& extension : extensions)
		{
			assert(extension.is_string());
			auto extensionName = extension.get<std::string>();
			if (extensionName == "mvp")
			{
				material->AddExtension(MaterialExtension::MODEL_VIEW_PROJECTION);
			}
			else if (extensionName == "time")
			{
				material->AddExtension(MaterialExtension::TIME);
			}
			else if (extensionName == "nearestLight")
			{
				material->AddExtension(MaterialExtension::NEAREST_LIGHT);
			}
			else if (extensionName == "ambient")
			{
				material->AddExtension(MaterialExtension::AMBIENT);
			}
			else if (extensionName == "camera")
			{
				material->AddExtension(MaterialExtension::CAMERA);
			}
		}

		return material;
	}

	Shader* DeserializeShader(const std::string& content)
	{
		auto shaderJsonObject = json::parse(content);
		
		Shader* shader = new Shader();
		std::string name = shaderJsonObject["name"].get<std::string>();
		std::string vertPath = shaderJsonObject["vert"].get<std::string>();
		std::string vertCode = StringUtils::ReadFile("shaders/" + vertPath);
		std::string fragPath = shaderJsonObject["frag"].get<std::string>();
		std::string fragCode = StringUtils::ReadFile("shaders/" + fragPath);
		shader->Init(name, vertCode, fragCode);

		return shader;
	}

	Mesh* DeserializeMesh(const std::string& content)
	{
		auto meshJsonObject = json::parse(content);
		std::string path = meshJsonObject["path"].get<std::string>();
		std::string type = meshJsonObject["type"].get<std::string>();

		if (type == "mesh")
		{
			Assimp::Importer importer;

			const aiScene* scene = importer.ReadFile("meshes/" + path,
				aiProcess_CalcTangentSpace
				| aiProcess_Triangulate
				| aiProcess_JoinIdenticalVertices
				| aiProcess_SortByPType
				| aiProcess_GenBoundingBoxes
				| aiProcess_GenNormals
				| aiProcess_OptimizeMeshes
				| aiProcess_GenUVCoords
				| aiProcess_OptimizeGraph
				// | aiProcess_MakeLeftHanded
			);

			if (!scene)
			{
				spdlog::info("assimp: {}", importer.GetErrorString());
				return nullptr;
			}

			Mesh* mesh = new Mesh();
			
			for (int i = 0; i < scene->mNumMeshes; i++)
			{
				// Process Mesh
				aiMesh* aiMesh = scene->mMeshes[i];
				SubMesh* subMesh = new SubMesh();

				subMesh->name = aiMesh->mName.C_Str();

				subMesh->AABBMin = glm::vec3(aiMesh->mAABB.mMin.x, aiMesh->mAABB.mMin.y, aiMesh->mAABB.mMin.z);
				subMesh->AABBMax = glm::vec3(aiMesh->mAABB.mMax.x, aiMesh->mAABB.mMax.y, aiMesh->mAABB.mMax.z);

				// Indices
				subMesh->indices.resize(aiMesh->mNumFaces * 3);
				for (size_t f = 0; f < aiMesh->mNumFaces; ++f)
				{
					for (size_t i = 0; i < 3; ++i)
					{
						subMesh->indices[f * 3 + i] = aiMesh->mFaces[f].mIndices[i];
					}
				}

				// Vertex
				subMesh->vertices.resize(aiMesh->mNumVertices);
				for (size_t i = 0; i < aiMesh->mNumVertices; ++i)
				{
					subMesh->vertices[i].position = glm::vec3(aiMesh->mVertices[i].x, aiMesh->mVertices[i].y, aiMesh->mVertices[i].z);
					subMesh->vertices[i].normal = glm::vec3(aiMesh->mNormals[i].x, aiMesh->mNormals[i].y, aiMesh->mNormals[i].z);
					subMesh->vertices[i].uv = glm::vec2(aiMesh->mTextureCoords[0][i].x, 1.0f - aiMesh->mTextureCoords[0][i].y);
					subMesh->vertices[i].tangent = glm::vec3(aiMesh->mTangents[i].x, aiMesh->mTangents[i].y, aiMesh->mTangents[i].z);
					subMesh->vertices[i].bitangent = glm::vec3(aiMesh->mBitangents[i].x, aiMesh->mBitangents[i].y, aiMesh->mBitangents[i].z);
				}

				mesh->AABBMin = glm::vec3(
					Math::Min(mesh->AABBMin.x, subMesh->AABBMin.x),
					Math::Min(mesh->AABBMin.y, subMesh->AABBMin.y),
					Math::Min(mesh->AABBMin.z, subMesh->AABBMin.z)
				);

				mesh->AABBMax = glm::vec3(
					Math::Min(mesh->AABBMax.x, subMesh->AABBMax.x),
					Math::Min(mesh->AABBMax.y, subMesh->AABBMax.y),
					Math::Min(mesh->AABBMax.z, subMesh->AABBMax.z)
				);

				mesh->children.push_back(subMesh);
			}
			
			mesh->CreateGPUResource();
;
			return mesh;
		}

		return nullptr;
	}
};
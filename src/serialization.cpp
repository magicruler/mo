#include "serialization.h"
#include "configs.h"
#include "json.hpp"
#include "string_utils.h"
#include "scene.h"
#include "actor.h"
#include "camera.h"
#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "shader.h"
#include "material.h"

#include "resources.h"

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
		actor->SetLocalPosition(DeserilizeVector3(transformObject["position"]));
		actor->SetLocalScale(DeserilizeVector3(transformObject["scale"]));
		actor->SetLocalRotation(DeserilizeVector3(transformObject["rotation"]));
	}

	void ProcessNode(json& node, Actor* currentActor)
	{
		auto typeName = node["type"].get<std::string>();
		
		if (typeName == "actor")
		{
			Actor* newActor = new Actor();
			auto name = node["name"].get<std::string>();
			newActor->SetName(name);
			
			/* 
			material, optional property 
			*/
			if (node.contains("material"))
			{
				auto materialPath = node["material"].get<std::string>();
				Material* material = Resources::GetMaterial(materialPath);
				newActor->SetMaterial(material);
			}

			/*
			mesh, optional property
			*/

			auto transformObject = node["transform"];
			ProcessTransformation(transformObject, newActor);
			
			currentActor->AddChild(newActor);

			auto childrenNode = node["children"];
			for (auto& childNode : childrenNode)
			{
				ProcessNode(childNode, newActor);
			}
		}
		else if (typeName == "camera")
		{
			Camera* newActor = new Camera();
			auto name = node["name"].get<std::string>();
			newActor->SetName(name);

			// Camera Properties
			newActor->fov = node["fov"];
			newActor->nearPlane = node["nearPlane"];
			newActor->farPlane = node["farPlane"];
			newActor->clearColor = DeserilizeVector4(node["clearColor"]);

			auto transformObject = node["transform"];
			ProcessTransformation(transformObject, newActor);

			currentActor->AddChild(newActor);

			auto childrenNode = node["children"];
			for (auto& childNode : childrenNode)
			{
				ProcessNode(childNode, newActor);
			}
		}
		// TODO
		else if (typeName == "pointLight")
		{

		}
	}

	Scene* DeserializeScene(const std::string& content)
	{
		auto sceneJsonObject = json::parse(content);
		Scene* scene = new Scene();
		
		auto children = sceneJsonObject["children"];
		for (auto& node : children)
		{
			ProcessNode(node, scene->rootNode);
		}

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

			texture->SetData2D(textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
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
};
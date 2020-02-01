#include "serialization.h"
#include "configs.h"
#include "json.hpp"
#include "string_utils.h"
#include "scene.h"
#include "actor.h"
#include "camera.h"

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
};
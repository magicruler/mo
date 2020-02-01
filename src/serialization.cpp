#include "serialization.h"
#include "configs.h"
#include "json.hpp"
#include "string_utils.h"
#include "scene.h"

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

	void ProcessNode(json& node, Scene* scene)
	{

	}

	Scene* DeserializeScene(const std::string& content)
	{
		auto sceneJsonObject = json::parse(content);
		Scene* scene = new Scene();
		
		auto children = sceneJsonObject["children"];
		for (auto& node : children)
		{
			ProcessNode(node, scene);
		}

		return scene;
	}
};
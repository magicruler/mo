#include "configs.h"

namespace Configuration
{
	float fps = 0.0f;
	std::string entryScene = "";
	std::vector<std::string> args;

	/*
	Store Layer Name Value Pair
	*/
	std::map<std::string, unsigned int> layers;

	unsigned int GetLayerMask(const std::string& layerName)
	{
		assert(layers.find(layerName) != layers.cend());
		return layers[layerName];
	}

	void SetLayerMask(const std::string& layerName, unsigned int layerValue)
	{
		layers[layerName] = layerValue;
	}

	float GetFPS()
	{
		return fps;
	}
	void SetFPS(float newFps)
	{
		fps = newFps;
	}
	std::string GetEntryScene()
	{
		return entryScene;
	}
	void SetEntryScene(const std::string& name)
	{
		entryScene = name;
	}

	std::string GetArg(int index)
	{
		return args[index];
	}

	int GetArgCount()
	{
		return args.size();
	}

	void PushArg(std::string arg)
	{
		args.push_back(arg);
	}
}
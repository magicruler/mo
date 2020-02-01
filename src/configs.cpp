#include "configs.h"

namespace Configuration
{
	float fps = 0.0f;
	std::string entryScene = "";
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
}
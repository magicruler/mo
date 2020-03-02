#pragma once
#include "common.h"

namespace Configuration
{
	unsigned int GetLayerMask(const std::string& layerName);
	void SetLayerMask(const std::string& layerName, unsigned int layerValue);

	float GetFPS();
	void SetFPS(float newFps);
	std::string GetEntryScene();
	void SetEntryScene(const std::string& name);
	std::string GetArg(int index);
	int GetArgCount();
	void PushArg(std::string arg);
}
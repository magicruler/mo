#pragma once
#include "common.h"

namespace Configuration
{
	float GetFPS();
	void SetFPS(float newFps);
	std::string GetEntryScene();
	void SetEntryScene(const std::string& name);
}
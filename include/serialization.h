#pragma once
#include "common.h"

class Scene;

namespace Serialization
{
	void LoadProject();
	Scene* DeserializeScene(const std::string& content);
};
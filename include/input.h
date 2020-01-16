#pragma once

#include "common.h"
#include "GLFW/glfw3.h"

enum class INPUT_KEY_ID
{
	CONTROL_LEFT = GLFW_KEY_LEFT_CONTROL,
	CONTROL_RIGHT = GLFW_KEY_RIGHT_CONTROL,

};



namespace Input
{
	void Init(GLFWwindow* win);
	bool GetKeyDown(INPUT_KEY_ID key);
}
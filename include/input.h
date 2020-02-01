#pragma once

#include <GLFW/glfw3.h>
#include "common.h"

enum class KEYBOARD_KEY
{
	ESC = GLFW_KEY_ESCAPE,
	F6 = GLFW_KEY_F6,
};

enum class MOUSE_KEY
{
	RIGHT = GLFW_MOUSE_BUTTON_RIGHT,
	LEFT = GLFW_MOUSE_BUTTON_LEFT,
	MIDDLE = GLFW_MOUSE_BUTTON_MIDDLE
};

enum class KEY_STATE
{
	PRESS = GLFW_PRESS,
	RELEASE = GLFW_RELEASE,
};

namespace Input
{
	void Init(GLFWwindow* window);
	void Update();
	KEY_STATE GetKeyState(KEYBOARD_KEY key);
	KEY_STATE GetMouseButtonState(MOUSE_KEY key);
	glm::vec2 GetMouseOffset();
	bool CheckKey(KEYBOARD_KEY key);
}

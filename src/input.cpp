#include "input.h"

GLFWwindow* currentWindow = nullptr;

namespace Input
{
	void Init(GLFWwindow* win)
	{
		currentWindow = win;
	}

	bool GetKeyDown(INPUT_KEY_ID key)
	{
		int state = glfwGetKey(currentWindow, (int)key);
		spdlog::info("KEY STATE IS: {}", state);
		return (GLFW_PRESS == state);
	};
}
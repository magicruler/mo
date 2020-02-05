#include "input.h"

namespace Input
{
	GLFWwindow* captured_window = nullptr;

	std::vector<bool> keyPress;
	std::list<int> keyNeedClear;

	bool keyDirty = false;
	bool mouseDirty = false;
	glm::vec2 mouseOffset = glm::vec2(0.0f, 0.0f);
	bool firstMouseCapture = true;
	float lastX = 0.0f;
	float lastY = 0.0f;

	void KeyCallback(GLFWwindow*, int key, int scancode, int action, int mods)
	{
		if (action == GLFW_PRESS)
		{
			if (key < 512)
			{
				keyPress[key] = true;
				keyNeedClear.push_back(key);
				keyDirty = true;
			}			
		}
	}

	void MouseCallback(GLFWwindow* window, double xpos, double ypos)
	{
		mouseDirty = true;
		if (firstMouseCapture)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouseCapture = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos;

		mouseOffset = glm::vec2(xoffset, yoffset);

		lastX = xpos;
		lastY = ypos;
	}

	void Init(GLFWwindow* window)
	{
		captured_window = window;
		keyPress.resize(512, false);
		glfwSetKeyCallback(captured_window, KeyCallback);
		glfwSetCursorPosCallback(captured_window, MouseCallback);
	}

	void Update()
	{
		if (keyDirty)
		{
			for (auto& index : keyNeedClear)
			{
				keyPress[index] = false;
			}
			keyNeedClear.clear();
			keyDirty = false;
		}

		if (mouseDirty)
		{
			mouseOffset = glm::vec2(0.0f, 0.0f);
			mouseDirty = false;
		}
	}

	KEY_STATE GetKeyState(KEYBOARD_KEY key)
	{
		return (KEY_STATE)glfwGetKey(captured_window, (int)key);
	}

	KEY_STATE GetMouseButtonState(MOUSE_KEY key)
	{
		return (KEY_STATE)glfwGetMouseButton(captured_window, (int)key);
	}

	glm::vec2 GetMouseOffset()
	{
		return mouseOffset;
	}

	bool CheckKey(KEYBOARD_KEY key)
	{
		bool result = keyPress[(int)key];
		return result;
	}
}


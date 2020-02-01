#include "time_manager.h"
#include <GLFW/glfw3.h>
#include "configs.h"
#include <chrono>
#include <thread>

namespace Time
{
	static float lastTime = 0.0f;
	static float deltaTime = 0.0f;

	void Update()
	{
		float currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;
	}

	float GetTime()
	{
		return glfwGetTime();
	}

	float GetDeltaTime()
	{
		return deltaTime;
	}

	void LimitFPS()
	{
		float expected_next_frame_time = (lastTime + 1.0f / Configuration::FPS);
		float currentTime = GetTime();

		if (currentTime < expected_next_frame_time)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<long long>(1000 * (expected_next_frame_time - currentTime))));
		}
	}
}
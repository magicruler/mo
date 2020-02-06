#pragma once
#include <assert.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Math
{
	constexpr float PI = 3.1415926535897932384626433f;
	constexpr float RAD_TO_DEGREE = 180.0f / PI;
	constexpr float DEGREE_TO_RAD = PI / 180.0f;
	constexpr float EPSILON = 0.00000000000000000001f;

	inline float Min(float a, float b)
	{
		if (a <= b)
		{
			return a;
		}

		return b;
	}

	inline float Max(float a, float b)
	{
		if (a >= b)
		{
			return a;
		}

		return b;
	}

	inline float Clamp(float val, float min, float max)
	{
		assert(min <= max);

		if (val <= min)
		{
			return min;
		}
		else if (val >= max)
		{
			return max;
		}

		return val;
	}

	inline float WrapClamp(float val, float min, float max)
	{
		assert(min <= max);

		if (val < min)
		{
			return max;
		}
		else if (val > max)
		{
			return min;
		}

		return val;
	}

	inline float Sign(float val)
	{
		if (val > 0.0f)
		{
			return 1;
		}
		else if (val < 0.0f)
		{
			return -1;
		}
		else
		{
			return 0;
		}
	}

	inline glm::vec3 DecomposeTranslation(const glm::mat4& matrix)
	{
		return glm::vec3(matrix[3][0], matrix[3][1], matrix[3][2]);
	}

	inline glm::vec3 DecomposeScale(const glm::mat4& matrix)
	{
		const float xs = (Sign(matrix[0][0] * matrix[0][1] * matrix[0][2] * matrix[0][3]) < 0) ? -1 : 1;
		const float ys = (Sign(matrix[1][0] * matrix[1][1] * matrix[1][2] * matrix[1][3]) < 0) ? -1 : 1;
		const float zs = (Sign(matrix[2][0] * matrix[2][1] * matrix[2][2] * matrix[2][3]) < 0) ? -1 : 1;

		return glm::vec3(
			xs* sqrtf(matrix[0][0] * matrix[0][0] + matrix[0][1] * matrix[0][1] + matrix[0][2] * matrix[0][2]),
			ys* sqrtf(matrix[1][0] * matrix[1][0] + matrix[1][1] * matrix[1][1] + matrix[1][2] * matrix[1][2]),
			zs* sqrtf(matrix[2][0] * matrix[2][0] + matrix[2][1] * matrix[2][1] + matrix[2][2] * matrix[2][2])
		);
	}

	inline glm::quat DecomposeRotation(const glm::mat4& matrix)
	{
		const glm::vec3 scale = DecomposeScale(matrix);

		// Avoid division by zero (we'll divide to remove scaling)
		if (scale.x == 0.0f || scale.y == 0.0f || scale.z == 0.0f) 
		{ 
			return glm::quat(0.0f, 0.0f, 0.0f, 1.0f); 
		}

		// Extract rotation and remove scaling
		glm::mat4 normalized;
		normalized[0][0] = matrix[0][0] / scale.x; normalized[0][1] = matrix[0][1] / scale.x; normalized[0][2] = matrix[0][2] / scale.x; normalized[0][3] = 0.0f;
		normalized[1][0] = matrix[1][0] / scale.y; normalized[1][1] = matrix[1][1] / scale.y; normalized[1][2] = matrix[1][2] / scale.y; normalized[1][3] = 0.0f;
		normalized[2][0] = matrix[2][0] / scale.z; normalized[2][1] = matrix[2][1] / scale.z; normalized[2][2] = matrix[2][2] / scale.z; normalized[2][3] = 0.0f;
		normalized[3][0] = 0; normalized[3][1] = 0; normalized[3][2] = 0; normalized[3][3] = 1.0f;

		return glm::quat_cast(normalized);
	}
}
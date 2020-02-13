#pragma once
#include <assert.h>
#include <glm/glm.hpp>
//#include "glm/vec2.hpp"
//#include "glm/vec3.hpp"
//#include "glm/vec4.hpp"
//#include "glm/mat2x2.hpp"
//#include "glm/mat3x3.hpp"
//#include "glm/mat4x4.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/euler_angles.hpp"

namespace Math
{
	constexpr float PI = 3.1415926535897932384626433f;
	constexpr float RAD_TO_DEGREE = 180.0f / PI;
	constexpr float DEGREE_TO_RAD = PI / 180.0f;
	constexpr float EPSILON = 0.00000000000000000001f;

	glm::vec3 EulerRotate(const glm::vec3& p, const glm::vec3& euler);

	inline float Abs(float a)
	{
		if (a < 0.0f)
		{
			return -a;
		}

		return a;
	}

	inline float AbsSum(const glm::vec3& v)
	{
		return Abs(v.x) + Abs(v.y) + Abs(v.z);
	}

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
}
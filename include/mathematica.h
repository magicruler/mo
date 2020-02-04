#pragma once
#include <assert.h>

namespace Math
{
	constexpr float PI = 3.1415926535897932384626433f;
	constexpr float RAD_TO_DEGREE = 180.0f / PI;
	constexpr float DEGREE_TO_RAD = PI / 180.0f;

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
}
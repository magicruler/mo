#include "mathematica.h"
#include <glm/gtx/rotate_vector.hpp>

namespace Math
{
	glm::vec3 EulerRotate(const glm::vec3& p, const glm::vec3& euler)
	{
		const glm::vec3 p1 = glm::rotateZ(p, glm::radians(euler.z));
		const glm::vec3 p2 = glm::rotateY(p1, glm::radians(euler.y));
		const glm::vec3 p3 = glm::rotateX(p2, glm::radians(euler.x));
		return p3;
	}
};
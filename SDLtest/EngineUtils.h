#pragma once

#include "Mesh.h"
#include <glm/gtx/norm.hpp>

namespace EngineUtils
{
	bool intersects(AABB a, AABB b);

	vec3 minDisplacement(AABB fixed, AABB movable);
};


#include "EngineUtils.h"

namespace EngineUtils {

	bool intersects(AABB a, AABB b)
	{
		if (a.position.x > b.size.x)
			return false;
		if (a.size.x < b.position.x)
			return false;

		if (a.position.y > b.size.y)
			return false;
		if (a.size.y < b.position.y)
			return false;

		if (a.position.z > b.size.z)
			return false;
		if (a.size.z < b.position.z)
			return false;

		return true;
	}

	vec3 minDisplacement(AABB fixed, AABB movable)
	{
		vec3 displacements[] = {
			vec3(fixed.size.x - movable.position.x, 0, 0),
			vec3(fixed.position.x - movable.size.x, 0, 0),

			vec3(0, fixed.size.y - movable.position.y, 0),
			vec3(0, fixed.position.y - movable.size.y, 0),

			vec3(0, 0, fixed.size.z - movable.position.z),
			vec3(0, 0, fixed.position.z - movable.size.z),
		};

		vec3* displacement = displacements;

		for (int i(0); i < 6; ++i) {
			if (length2(displacements[i]) <= length2(*displacement)) {
				displacement = &displacements[i];
			}
		}

		return *displacement;
	}

}
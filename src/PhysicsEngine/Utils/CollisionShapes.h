#pragma once

#include "PhysicsEngine/Shapes/Box.h"
#include "PhysicsEngine/Shapes/Sphere.h"

namespace Physics {
	namespace Utils {
		bool Collision(const Box* A, const Sphere* B, glm::vec3 position);
	}
}



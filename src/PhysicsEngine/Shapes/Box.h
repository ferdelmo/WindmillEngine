#pragma once

#include "Shape.h"

#include <vector>

namespace Physics {

	const std::vector<glm::vec3> boxVertex = {
		{0.5,-0.5, -0.5}, {-0.5,-0.5, -0.5}, {-0.5,0.5, -0.5}, {-0.5,0.5, -0.5},
		{0.5,-0.5, 0.5}, {-0.5,-0.5, 0.5}, {-0.5,0.5, 0.5}, {-0.5,0.5, 0.5}
	};

	class Box : public Shape
	{
	};
}


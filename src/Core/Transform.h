#pragma once

#include "Core/Vector3.h"
#include "glm/glm.hpp"

/*
	Class to represent a transform, compose by position, rotation and scale

	TODO: add utils to operate with them
	TODO: add rotation and scale

	@author Fernando del Molino
*/
class Transform
{
public:
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;
};


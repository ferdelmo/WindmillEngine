#pragma once

#include <glm/glm.hpp>

namespace Physics {

	enum class ShapeType {
		SPHERE,
		BOX,
		UNDEFINED
	};
	/*
		Class to represent any shape in the physics engine. Is an abstract to class to implement methods
	*/
	class Shape
	{
	public:
		virtual ~Shape(){}

		virtual ShapeType GetType() const {
			return ShapeType::UNDEFINED;
		}

		static bool Collision(Shape* a, Shape* b, glm::vec3& position);
	};
}

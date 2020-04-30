#pragma once

#include "Shape.h"

#include <glm/glm.hpp>


namespace Physics {
	/*
		Sphere shape
	*/
	class Sphere : public Shape
	{
	private: 
		glm::vec3 pos;
		float r;

	public:
		virtual ShapeType GetType() const override{
			return ShapeType::SPHERE;
		}

		Sphere(glm::vec3 pos, float r);

		static bool Collision(const Sphere* a, const Sphere* b, glm::vec3& position);

		void SetPosition(glm::vec3 pos);
		glm::vec3 GetPosition() const;

		void SetRadius(float r);
		float GetRadius() const;

	};
}


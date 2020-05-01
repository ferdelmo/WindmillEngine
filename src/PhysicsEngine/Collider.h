#pragma once

#include <vector>
#include "Shapes/Shape.h"

namespace Physics {

	/*
		Collider represent by a shape
	*/
	class Collider
	{
	public: 
		Collider(glm::vec3 position, float r);

		Collider(glm::vec3 position, glm::vec3 extent);

		virtual ~Collider();

		/*
			Call by the physics engine when an object start overlapping another
		*/
		void BeginOverlap(Collider* col);

		/*
			Call by the physics engine when an object is overlapping another
		*/
		void IsOverlapping(Collider* col);

		/*
			Call by the physics engine when an object is overlapping another
		*/
		void EndOverlap(Collider* col);

		Shape* GetShape();

		std::vector<Collider*>& GetOverlappingColliders();

	private:
		Shape* _shape;

		std::vector<Collider*> _overlappingColliders;
	};
}


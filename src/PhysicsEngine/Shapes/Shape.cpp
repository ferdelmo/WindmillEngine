#include "Shape.h"

#include "Sphere.h"

using namespace Physics;

bool Shape::Collision(Shape* a, Shape* b, glm::vec3& position) {
	if (a->GetType() == b->GetType() && a->GetType() == ShapeType::SPHERE) {
		return Sphere::Collision(static_cast<Sphere*>(a), static_cast<Sphere*>(b), position);
	}
	else {
		return false;
	}
}
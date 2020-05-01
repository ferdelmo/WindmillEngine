#include "Shape.h"

#include "Sphere.h"
#include "Box.h"
#include "PhysicsEngine/Utils/CollisionShapes.h"

using namespace Physics;

bool Shape::Collision(Shape* a, Shape* b, glm::vec3& position) {
	if (a->GetType() == b->GetType() && a->GetType() == ShapeType::SPHERE) {
		return Sphere::Collision(static_cast<Sphere*>(a), static_cast<Sphere*>(b), position);
	}
	else if (a->GetType() == b->GetType() && a->GetType() == ShapeType::BOX) {
		return Box::Collision(static_cast<Box*>(a), static_cast<Box*>(b), position);
	}
	else if (a->GetType() == ShapeType::SPHERE && b->GetType() == ShapeType::BOX){

		return Utils::Collision(static_cast<Box*>(b), static_cast<Sphere*>(a), position);
	}
	else if (a->GetType() == ShapeType::BOX && b->GetType() == ShapeType::SPHERE) {
		return Utils::Collision(static_cast<Box*>(a), static_cast<Sphere*>(b), position);
	}
	else {
		return false;
	}
}
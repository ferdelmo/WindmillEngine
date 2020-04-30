#include "Collider.h"

#include <algorithm>

#include "Shapes/Sphere.h"

#include <iostream>

using namespace Physics;

Collider::Collider(glm::vec3 position, float r) {
	_shape = new Sphere(position, r);
}

Collider::~Collider() {
	delete _shape;
}

/*
	Call by the physics engine when an object start overlapping another
*/
void Collider::BeginOverlap(Collider* col) {

	std::cout << this << " BEGIN OVERLAP WITH " << col << std::endl;

	_overlappingColliders.push_back(col);
}

/*
	Call by the physics engine when an object is overlapping another
*/
void Collider::IsOverlapping(Collider* col) {

}

/*
	Call by the physics engine when an object is overlapping another
*/
void Collider::EndOverlap(Collider* col) {
	std::vector<Collider*>::iterator it = std::find(_overlappingColliders.begin(), _overlappingColliders.end(), col);
	
	if (it != _overlappingColliders.end()) {
		_overlappingColliders.erase(it);
	}

	std::cout << this << " END OVERLAP WITH " << col << std::endl;
}


Shape* Collider::GetShape() {
	return _shape;
}

std::vector<Collider*>& Collider::GetOverlappingColliders() {
	return _overlappingColliders;
}
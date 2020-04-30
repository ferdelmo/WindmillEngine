#include "PhysicsEngine.h"

#include "Collider.h"

using namespace Physics;

PhysicsEngine* PhysicsEngine::_instance = nullptr;

PhysicsEngine& PhysicsEngine::GetInstance() {
	if (_instance == nullptr) {
		_instance = new PhysicsEngine();
	}
	return *_instance;
}

void PhysicsEngine::CleanUp() {
	delete _instance;
}


PhysicsEngine::PhysicsEngine() {

}

PhysicsEngine::~PhysicsEngine() {

}

void PhysicsEngine::AddCollider(Collider* col) {
	_colliders.push_back(col);
}

void PhysicsEngine::RemoveCollider(Collider* col) {
	std::vector<Collider*>::iterator it = std::find(_colliders.begin(), _colliders.end(), col);

	if (it != _colliders.end()) {
		_colliders.erase(it);
	}
}

/*
	Check for collisions in the colliders and update them
*/
void PhysicsEngine::Update() {
	// compare all the colliders with each other
	if (_colliders.size() == 0) {
		return;
	}
	for (int i = 0; i < (_colliders.size() - 1); i++) {
		for (int j = i + 1; j < _colliders.size(); j++) {
			Collider* a = _colliders[i];
			Collider* b = _colliders[j];

			glm::vec3 point = { 0,0,0 };
			if (Shape::Collision(a->GetShape(), b->GetShape(), point)) {
				//are overlapping
				if (std::find(a->GetOverlappingColliders().begin(),
					a->GetOverlappingColliders().end(), b) != a->GetOverlappingColliders().end()) {
					//If was already in the list of overlapping, no new collision
					a->IsOverlapping(b);
					b->IsOverlapping(a);
				}
				else {
					//first time they overlpap, add each other to the list of colliding objects and call begin overlap

					a->BeginOverlap(b);
					b->BeginOverlap(a);
				}
			}
			else {
				//are not overlapping
				// check if last check they was overlapping, to call end overlap and erase each other from
				// the collider list
				if (std::find(a->GetOverlappingColliders().begin(),
					a->GetOverlappingColliders().end(), b) != a->GetOverlappingColliders().end()) {

					a->EndOverlap(b);
					b->EndOverlap(a);
				}
			}
		}
	}

}
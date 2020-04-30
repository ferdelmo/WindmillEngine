#include "SphereColliderComponent.h"

#include "Engine/GameObject.h"

#include "PhysicsEngine/PhysicsEngine.h"

using namespace Physics;

SphereColliderComponent::SphereColliderComponent(glm::vec3 position, float r) :
	_col(position, r){
	_sphere = static_cast<Sphere*>(_col.GetShape());
}

SphereColliderComponent::~SphereColliderComponent() {

}

void SphereColliderComponent::Start() {
	PhysicsEngine::GetInstance().AddCollider(&_col);
}

void SphereColliderComponent::End() {
	PhysicsEngine::GetInstance().RemoveCollider(&_col);
}

void SphereColliderComponent::Update(float deltaTime) {
	_sphere->SetPosition(_owner->transform.position);
}
#include "SphereColliderComponent.h"

#include "Engine/GameObject.h"

#include "PhysicsEngine/PhysicsEngine.h"

#include <iostream>

using namespace Physics;

SphereColliderComponent::SphereColliderComponent(glm::vec3 position, float r) :
	_col(position, r){
	_sphere = static_cast<Sphere*>(_col.GetShape());
}

SphereColliderComponent::~SphereColliderComponent() {

}

void SphereColliderComponent::Start() {
	_col.SetGameObject(_owner);
	_col.GetBeginOverlapEvents().Bind([this](GameObject* go, Collider* col) {
		std::cout << _owner << " colliding with gameobject " << go << std::endl;
		});
	PhysicsEngine::GetInstance().AddCollider(&_col);
}

void SphereColliderComponent::End() {
	PhysicsEngine::GetInstance().RemoveCollider(&_col);
}

void SphereColliderComponent::Update(float deltaTime) {
	_sphere->SetPosition(_owner->transform.position);
}
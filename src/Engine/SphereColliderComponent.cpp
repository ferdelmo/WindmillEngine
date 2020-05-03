#include "SphereColliderComponent.h"

#include "Engine/GameObject.h"

#include "PhysicsEngine/PhysicsEngine.h"

#include <functional>

#include <iostream>

using namespace Physics;

SphereColliderComponent::SphereColliderComponent(glm::vec3 position, float r) :
	_col(position, r){
	_sphere = static_cast<Sphere*>(_col.GetShape());
}

SphereColliderComponent::~SphereColliderComponent() {

}

void SphereColliderComponent::Initialize() {
	_col.SetGameObject(_owner);

	using std::placeholders::_1;
	using std::placeholders::_2;
	_col.GetBeginOverlapEvents().Bind(std::bind(&SphereColliderComponent::BeginOverlap, this, _1, _2));
	_col.GetEndOverlapEvents().Bind(std::bind(&SphereColliderComponent::EndOverlap, this, _1, _2));

	PhysicsEngine::GetInstance().AddCollider(&_col);
}

void SphereColliderComponent::Start() {

}

void SphereColliderComponent::End() {
	PhysicsEngine::GetInstance().RemoveCollider(&_col);
}

void SphereColliderComponent::Update(float deltaTime) {
	_sphere->SetPosition(_owner->transform.position);
}


void SphereColliderComponent::BeginOverlap(GameObject* go, Collider* co) {
	std::cout << "Sphere " << _owner << " begin overlap with gameobject " << go << std::endl;
}


void SphereColliderComponent::EndOverlap(GameObject* go, Physics::Collider* co) {
	std::cout << "Sphere " << _owner << " end overlap with gameobject " << go << std::endl;
}

void SphereColliderComponent::BindBeginOverlap(const BeginOverlapEvent& boe) {
	_col.GetBeginOverlapEvents().Bind(boe);
}

void SphereColliderComponent::EndBeginOverlap(const EndOverlapEvent& eoe) {
	_col.GetEndOverlapEvents().Bind(eoe);
}
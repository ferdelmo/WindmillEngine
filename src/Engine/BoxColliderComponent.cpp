#include "BoxColliderComponent.h"

#include "Engine/GameObject.h"

#include "PhysicsEngine/PhysicsEngine.h"

using namespace Physics;

BoxColliderComponent::BoxColliderComponent(glm::vec3 position, glm::vec3 extents) :
	_col(position, extents) {
	_box = static_cast<Box*>(_col.GetShape());
}

BoxColliderComponent::~BoxColliderComponent() {

}


void BoxColliderComponent::Initialize() {
	_col.SetGameObject(_owner);

	using std::placeholders::_1;
	using std::placeholders::_2;
	_col.GetBeginOverlapEvents().Bind(std::bind(&BoxColliderComponent::BeginOverlap, this, _1, _2));
	_col.GetEndOverlapEvents().Bind(std::bind(&BoxColliderComponent::EndOverlap, this, _1, _2));

	PhysicsEngine::GetInstance().AddCollider(&_col);
}

void BoxColliderComponent::Start() {

}

void BoxColliderComponent::End() {
	PhysicsEngine::GetInstance().RemoveCollider(&_col);
}

void BoxColliderComponent::Update(float deltaTime) {
	_box->SetPosition(_owner->transform.position);

	Transform trans = _owner->transform;
	glm::normalize(trans.rotation);

	glm::mat4 rot = glm::toMat4(trans.rotation);

	_box->SetModel(glm::translate(glm::mat4(1), trans.position) * rot);
}


void BoxColliderComponent::BeginOverlap(GameObject* go, Physics::Collider* co) {

}

void BoxColliderComponent::EndOverlap(GameObject* go, Physics::Collider* co) {

}


void BoxColliderComponent::BindBeginOverlap(const BeginOverlapEvent& boe) {
	_col.GetBeginOverlapEvents().Bind(boe);
}

void BoxColliderComponent::EndBeginOverlap(const EndOverlapEvent& eoe) {
	_col.GetEndOverlapEvents().Bind(eoe);
}
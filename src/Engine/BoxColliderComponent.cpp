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

void BoxColliderComponent::Start() {
	_col.SetGameObject(_owner);
	PhysicsEngine::GetInstance().AddCollider(&_col);
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
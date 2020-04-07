#include "CapsuleCollider.h"
#include "Engine/GameObject.h"

CapsuleCollider::CapsuleCollider() : _focus1(glm::vec3(0.0f, 0.0f, 0.0f)), _focus2(glm::vec3(0.0f, 0.0f, 0.0f)),
				_center(glm::vec3(0.0f, 0.0f, 0.0f)), _radius(0.0f), _axis(glm::vec3(0.0f, 0.0f, 0.0f)), 
				_semiHeight(0.0f) {}

CapsuleCollider::CapsuleCollider(glm::vec3 focus1, glm::vec3 focus2, float radius) :
				_focus1(focus1), _focus2(focus2), _radius(radius) {
	_semiHeight = glm::distance(_focus1, _focus2) / 2.0f;
	float dist = glm::distance(_focus1, _focus2);
	glm::vec3 dir = _focus2 - _focus1;
	dir = glm::normalize(dir);
	_center = _focus1 + dir * (dist / 2.0f);
	_axis = dir;
}

CapsuleCollider::CapsuleCollider(glm::vec3 center, float radius, float semiHeight, glm::vec3 axis) :
				_center(center), _radius(radius), _axis(glm::normalize(axis)), _semiHeight(semiHeight) {
	
	_focus1 = center - _axis * semiHeight;
	_focus2 = center + _axis * semiHeight;

}

CapsuleCollider::~CapsuleCollider(){}

void CapsuleCollider::Initialize(){}

void CapsuleCollider::Start() {
	_center = _owner->transform.position;
	_focus1 = _center - _axis * _semiHeight;
	_focus2 = _center + _axis * _semiHeight;
}

void CapsuleCollider::Update(float deltaTime) {
	_center = _owner->transform.position;
	_focus1 = _center - _axis * _semiHeight;
	_focus2 = _center + _axis * _semiHeight;
}

void CapsuleCollider::End(){}

glm::vec3 CapsuleCollider::GetFocus1() const {
	return _focus1;
}

void CapsuleCollider::SetFocus1(glm::vec3 focus1) {
	this->_focus1 = focus1;
}

glm::vec3 CapsuleCollider::GetFocus2() const {
	return _focus2;
}

void CapsuleCollider::SetFocus2(glm::vec3 focus2) {
	this->_focus2 = focus2;
}

glm::vec3 CapsuleCollider::GetCenter() const {
	return _center;
}

float CapsuleCollider::GetRadius() const {
	return _radius;
}

void CapsuleCollider::SetRadius(float radius) {
	this->_radius = radius;
}

glm::vec3 CapsuleCollider::GetAxis() const {
	return _axis;
}

float CapsuleCollider::GetSemiHeight() const {
	return _semiHeight;
}
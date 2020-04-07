#include "SphereCollider.h"
#include "Engine/GameObject.h"
#include "CapsuleCollider.h"

SphereCollider::SphereCollider() : _center(glm::vec3(0.0f,0.0f,0.0f)), _radius(0.0f) {}

SphereCollider::SphereCollider(float radius) : _radius(radius), _center(glm::vec3(0.0f, 0.0f, 0.0f)) {}

SphereCollider::SphereCollider(glm::vec3 center, float radius) : _center(center), _radius(radius) {}

SphereCollider::~SphereCollider() {

}

void SphereCollider::Initialize() {

 }

void SphereCollider::Start() {
    _center = _owner->transform.position;
}

void SphereCollider::Update(float deltaTime) {
    _center = _owner->transform.position;
}

void SphereCollider::End() {

}

glm::vec3 SphereCollider::GetCenter() const {
    return _center;
}

void SphereCollider::SetCenter(glm::vec3 center) {
    this->_center = center;
}

float SphereCollider::GetRadius() const {
    return _radius;
}

void SphereCollider::SetRadius(float radius) {
    this->_radius = radius;
}

bool SphereCollider::Collision(const SphereCollider* b) {
    float sumRadius = this->_radius + b->GetRadius();
    return glm::distance(this->_center, b->GetCenter()) < sumRadius;
}

bool SphereCollider::Collision(const CapsuleCollider* b) {
    float sumRadius = this->_radius + b->GetRadius();

    // vector and distance from sphere to focus 1
    glm::vec3 sphereToFocus1 = b->GetFocus1() - this->_center;
    float sphereToFocus1Dist = glm::length(sphereToFocus1);
    sphereToFocus1 = glm::normalize(sphereToFocus1);

    if (sphereToFocus1Dist < sumRadius) {
        return true;
    }

    // vector and distance from sphere to focus 2
    glm::vec3 sphereToFocus2 = b->GetFocus2() - this->_center;
    float sphereToFocus2Dist = glm::length(sphereToFocus2);
    sphereToFocus2 = glm::normalize(sphereToFocus2);

    if (sphereToFocus2Dist < sumRadius) {
        return true;
    }

    // vector and distance from sphere to capsule center
    glm::vec3 sphereToCenter = b->GetCenter() - this->_center;
    float sphereToCenterDist = glm::length(sphereToCenter);
    sphereToCenter = glm::normalize(sphereToCenter);

    if (sphereToCenterDist < sumRadius) {
        return true;
    }

    // distance to the capsule
    float dot = glm::dot(sphereToFocus1, b->GetAxis());
    float dist;
    if (dot > 0) {
        float adjacent = dot * (b->GetSemiHeight() * 2.0f);
        dist = glm::sqrt(sphereToFocus1Dist * sphereToFocus1Dist - adjacent * adjacent);
        if (dist < sumRadius) {
            return true;
        }
    }

    dot = glm::dot(sphereToFocus2, b->GetAxis());
    if (dot > 0) {
        float adjacent = dot * (b->GetSemiHeight() * 2.0f);
        dist = glm::sqrt(sphereToFocus2Dist * sphereToFocus2Dist - adjacent * adjacent);
        if (dist < sumRadius) {
            return true;
        }
    }
    return false;
}
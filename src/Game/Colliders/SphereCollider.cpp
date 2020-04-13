#include "SphereCollider.h"
#include "Engine/GameObject.h"
#include "CapsuleCollider.h"
#include <iostream>
#include "Game/Managers/PhysicsManager.h"
SphereCollider::SphereCollider() : _center(glm::vec3(0.0f,0.0f,0.0f)), _radius(0.0f) {}

SphereCollider::SphereCollider(float radius) : _radius(radius), _center(glm::vec3(0.0f, 0.0f, 0.0f)) {}

SphereCollider::SphereCollider(glm::vec3 center, float radius) : _center(center), _radius(radius) {}

SphereCollider::~SphereCollider() {
    Physics::PhysicsManager::GetInstance().RemoveSphereCollider(this);
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
    glm::vec3 sphereToFocus1 = this->_center - b->GetFocus1();
    float sphereToFocus1Dist = glm::length(sphereToFocus1);
    sphereToFocus1 = glm::normalize(sphereToFocus1);

    if (sphereToFocus1Dist < sumRadius) {
       // std::cout << "focus 1" << std::endl;
        return true;
    }

    // vector and distance from sphere to focus 2
    glm::vec3 sphereToFocus2 = this->_center - b->GetFocus2();
    float sphereToFocus2Dist = glm::length(sphereToFocus2);
    sphereToFocus2 = glm::normalize(sphereToFocus2);

    if (sphereToFocus2Dist < sumRadius) {
       // std::cout << "focus 2" << std::endl;
        return true;
    }

    // vector and distance from sphere to capsule center
    glm::vec3 sphereToCenter = b->GetCenter() - this->_center;
    float sphereToCenterDist = glm::length(sphereToCenter);
    sphereToCenter = glm::normalize(sphereToCenter);

    if (sphereToCenterDist < sumRadius) {
        //std::cout << "center" << std::endl;
        return true;
    }

    // distance to the capsule
    glm::vec3 capsuleAxis = b->GetFocus2() - b->GetFocus1();
    capsuleAxis = glm::normalize(capsuleAxis);
    float dot = glm::dot(sphereToFocus1, capsuleAxis);
    float dist;
   /* std::cout <<"sphereToFocus1: " << sphereToFocus1.x << ", " << sphereToFocus1.y << ", " <<
        sphereToFocus1.z << std::endl;*/
    // std::cout << dot << std::endl;
    if (dot > 0) {
        float adjacent = dot * sphereToFocus1Dist;
        dist = glm::sqrt(sphereToFocus1Dist * sphereToFocus1Dist - adjacent * adjacent);
        if (adjacent < b->GetSemiHeight() * 2.0f && dist < sumRadius) {
           // std::cout << "first dot" << std::endl;
            return true;
        }
    }

    capsuleAxis = -1.0f * capsuleAxis;
    dot = glm::dot(sphereToFocus2, capsuleAxis);
    if (dot > 0) {
        float adjacent = dot * sphereToFocus2Dist;
        dist = glm::sqrt(sphereToFocus2Dist * sphereToFocus2Dist - adjacent * adjacent);
        if (adjacent < b->GetSemiHeight() * 2.0f && dist < sumRadius) {
           // std::cout << "second dot" << std::endl;
            return true;
        }
    }
    return false;
}

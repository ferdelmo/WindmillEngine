#pragma once

#include "Component.h"

#include "PhysicsEngine/Collider.h"
#include "PhysicsEngine/Shapes/Sphere.h"

#include <glm/glm.hpp>

class SphereColliderComponent : public Component
{
public:
	SphereColliderComponent(glm::vec3 position, float r);

	~SphereColliderComponent();

	void Start() override;

	void End() override;

	void Update(float deltaTime) override;

private:
	Physics::Collider _col;

	Physics::Sphere* _sphere;
};


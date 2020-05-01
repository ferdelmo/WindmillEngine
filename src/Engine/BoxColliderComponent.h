#pragma once

#include "Component.h"

#include "PhysicsEngine/Collider.h"
#include "PhysicsEngine/Shapes/Box.h"

#include <glm/glm.hpp>

class BoxColliderComponent : public Component
{
public:
	BoxColliderComponent(glm::vec3 position, glm::vec3 extents);

	~BoxColliderComponent();

	void Start() override;

	void End() override;

	void Update(float deltaTime) override;

private:
	Physics::Collider _col;

	Physics::Box* _box;
};


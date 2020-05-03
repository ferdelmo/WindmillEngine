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

	void Initialize() override;

	void Start() override;

	void End() override;

	void Update(float deltaTime) override;

	void BeginOverlap(GameObject* go, Physics::Collider* co);

	void EndOverlap(GameObject* go, Physics::Collider* co);

	void BindBeginOverlap(const Physics::BeginOverlapEvent& boe);
	void EndBeginOverlap(const Physics::EndOverlapEvent& eoe);

private:
	Physics::Collider _col;

	Physics::Sphere* _sphere;
};


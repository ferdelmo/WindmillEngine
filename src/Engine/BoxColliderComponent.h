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

	Physics::Box* _box;
};


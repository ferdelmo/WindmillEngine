#pragma once

#include "Engine/Component.h"
#include <glm/glm.hpp>

class CapsuleCollider;

class SphereCollider : public Component {

public:
	SphereCollider();

	SphereCollider(float radius);
	
	SphereCollider(glm::vec3 center, float radius);

	~SphereCollider();

	virtual void Initialize() override;

	virtual void Start() override;

	virtual void Update(float deltaTime) override;

	virtual void End() override;

	glm::vec3 GetCenter() const;

	void SetCenter(glm::vec3 center);

	float GetRadius() const;

	void SetRadius(float radius);

	bool Collision(const SphereCollider* b);

	bool Collision(const CapsuleCollider* b);

private:
	glm::vec3 _center;

	float _radius;
};
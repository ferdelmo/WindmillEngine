#pragma once
#include "Engine/Component.h"
#include <glm/glm.hpp>

class CapsuleCollider : public Component {
	
public:
	CapsuleCollider();

	CapsuleCollider(glm::vec3 focus1, glm::vec3 focus2, float radius);

	CapsuleCollider(glm::vec3 center, float radius, float semiHeight, glm::vec3 axis);

	~CapsuleCollider();

	virtual void Initialize() override;

	virtual void Start() override;

	virtual void Update(float deltaTime) override;

	virtual void End() override;

	glm::vec3 GetFocus1() const;

	void SetFocus1(glm::vec3 focus1);

	glm::vec3 GetFocus2() const;

	void SetFocus2(glm::vec3 focus2);

	glm::vec3 GetCenter() const;

	float GetRadius() const;

	void SetRadius(float radius);

	// Director vector from focus 1 to focus 2
	glm::vec3 GetAxis() const;

	float GetSemiHeight() const;

private:

	glm::vec3 _focus1,  _focus2;
	
	glm::vec3 _center;
	
	float _radius;

	glm::vec3 _axis;

	float _semiHeight;
};
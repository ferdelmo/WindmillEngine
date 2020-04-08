#pragma once
#include "Engine/GameObject.h"

class Camera;

class FirstPersonPlayer : public GameObject
{
private:
	glm::vec3 directionLookAt = glm::vec3(0,1,0);
public:

	void SetLookAt(glm::vec3 lookAt);

	glm::vec3 GetLookAt() const;

	virtual void Update(float deltaTime) override;
};


#pragma once
#include "Engine/GameObject.h"

#include "RenderEngine/Renderer/Uniform.h"

class Camera;

class FirstPersonPlayer : public GameObject
{
private:
	glm::vec3 directionLookAt = glm::vec3(0,1,0);

	PointLight& _pLight;
public:

	FirstPersonPlayer(PointLight& personalLight);


	void SetLookAt(glm::vec3 lookAt);

	glm::vec3 GetLookAt() const;

	virtual void Update(float deltaTime) override;
};


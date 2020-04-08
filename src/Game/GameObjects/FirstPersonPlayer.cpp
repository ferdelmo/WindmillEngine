#include "FirstPersonPlayer.h"

#include "Engine/World.h"

#include <glm/gtc/matrix_transform.hpp>

void FirstPersonPlayer::SetLookAt(glm::vec3 lookAt) {

}

void FirstPersonPlayer::Update(float deltaTime) {
	Camera& cam = GetWorld()->GetCamera();

	glm::vec3 aux = glm::cross(directionLookAt, glm::vec3(0, 0, 1));

	glm::vec3 up = glm::cross(aux, directionLookAt);

	cam.SetView(glm::lookAt(
		transform.position,
		transform.position + directionLookAt,
		up
	));

	GameObject::Update(deltaTime);

}
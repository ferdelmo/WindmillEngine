#include "FirstPersonPlayer.h"

#include "Engine/World.h"

#include "Input/InputManager.h"
#include <glm/gtc/matrix_transform.hpp>

glm::vec3 FirstPersonPlayer::GetLookAt() const {
	return glm::normalize(directionLookAt);
}

void FirstPersonPlayer::SetLookAt(glm::vec3 lookAt) {
	// do nothing
	directionLookAt = lookAt; 
	Input::InputManager::GetInstance().ResetCursorPosition();
}

void FirstPersonPlayer::Update(float deltaTime) {

	GameObject::Update(deltaTime);

	Camera& cam = GetWorld()->GetCamera();

	glm::vec3 aux = glm::cross(directionLookAt, glm::vec3(0, 0, 1));

	glm::vec3 up = glm::cross(aux, directionLookAt);

	cam.SetView(glm::lookAt(
		transform.position,
		transform.position + directionLookAt,
		-up
	));

}
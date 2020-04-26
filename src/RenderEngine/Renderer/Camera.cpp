#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/compatibility.hpp>

#include "Input/InputManager.h"

#include <iostream>

Camera::Camera() {
	_view = glm::lookAt(glm::vec3(0.0f, -2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	_proj = glm::perspective(glm::radians(45.0f), 16 / 9.0f, 0.1f, 10.0f);

	_proj[1][1] *= -1;
}

Camera::Camera(glm::vec3 eyePosition, glm::vec3 center, glm::vec3 upAxis, float fov, float aspectRatio, float near, float far) :
	aspectRatio(aspectRatio), near(near), far(far), fov(fov) {
	_proj = glm::perspective(glm::radians(fov), aspectRatio, near, far);

	position = eyePosition;
	lookAt = (center - position);
	lookAt = glm::normalize(lookAt);

	_view = glm::lookAt(position, position + lookAt, upAxis);

	_proj[1][1] *= -1;
}

glm::mat4 Camera::GetView() const{
	return _view;
}

glm::mat4 Camera::GetProjection() const{
	return _proj;
}

void Camera::Update(float deltaTime) {
	static bool firstCtrl = true;
	static bool firstClick = true;

	if (Input::InputManager::GetInstance().IsKeyPressed(GLFW_KEY_1)) {
		movSpeed += 2.5 * deltaTime;
	}

	if (Input::InputManager::GetInstance().IsKeyPressed(GLFW_KEY_2)) {
		movSpeed -= 2.5 * deltaTime;
	}

	if (Input::InputManager::GetInstance().IsKeyPressed(GLFW_MOUSE_BUTTON_RIGHT)) {
		//move camera
		if (firstCtrl) {
			glfwSetInputMode(VulkanInstance::GetInstance().window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
			Input::InputManager::GetInstance().GetInstance().ResetCursorPosition();
			firstCtrl = false;
			firstClick = true;
		}
		OrientFirstPersonCamera(deltaTime);
	}
	else {
		// show cursor and click on things
		if (firstClick) {
			glfwSetInputMode(VulkanInstance::GetInstance().window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			Input::InputManager::GetInstance().GetInstance().ResetCursorPosition();
			firstClick = false;
			firstCtrl = true;
		}
	}
	MoveFirstPersonCamera(deltaTime);
	FOVFirstPersonCamera(deltaTime);

}

void Camera::MoveFirstPersonCamera(float deltaTime) {

	glm::vec3 defaultUp = { 0.0, 0.0, 1.0 };
	glm::vec3 right = glm::cross(lookAt, defaultUp);
	glm::vec3 up = glm::cross(right, lookAt);


	glm::vec3 movement = { 0, 0, 0 };
	/*
		Camera position movement
	*/
	if (Input::InputManager::GetInstance().IsKeyPressed(GLFW_KEY_W)) {
		movement += lookAt;
	}
	if (Input::InputManager::GetInstance().IsKeyPressed(GLFW_KEY_S)) {
		movement += -lookAt ;
	}
	if (Input::InputManager::GetInstance().IsKeyPressed(GLFW_KEY_A)) {
		movement += -right;
	}
	if (Input::InputManager::GetInstance().IsKeyPressed(GLFW_KEY_D)) {
		movement += right;
	}
	if (Input::InputManager::GetInstance().IsKeyPressed(GLFW_KEY_E)) {
		movement += up;
	}
	if (Input::InputManager::GetInstance().IsKeyPressed(GLFW_KEY_Q)) {
		movement += -up;
	}

	if (movement != glm::vec3({0, 0, 0})) {
		movement = glm::normalize(movement);
	}

	position += movement * movSpeed * deltaTime;

	_view = glm::lookAt(position, position + lookAt, up);

}

void Camera::OrientFirstPersonCamera(float deltaTime) {
	/*
		Change look at
	*/
	lookAt = glm::normalize(lookAt);

	double deltaMouse[] = { 0, 0 };
	Input::InputManager::GetInstance().GetMousePosition(deltaMouse[0], deltaMouse[1]);

	deltaMouse[0] = deltaMouse[0] / VulkanInstance::GetInstance().width;
	deltaMouse[1] = deltaMouse[1] / VulkanInstance::GetInstance().height;

	deltaMouse[0] -= 0.5;
	deltaMouse[1] -= 0.5;

	deltaMouse[0] *= 2;
	deltaMouse[1] *= 2;

	if (std::abs(deltaMouse[0]) < minSensibility * deltaTime) {
		deltaMouse[0] = 0;
	}

	if (std::abs(deltaMouse[1]) < minSensibility * deltaTime) {
		deltaMouse[1] = 0;
	}

	glm::vec2 look = {

		-deltaMouse[0] * angularSpeed * deltaTime,

		-deltaMouse[1] * angularSpeed * deltaTime

	};

	float currentDeclination = std::acos(lookAt.z);
	float requestedDeclination = currentDeclination + look.y;

	/*
	std::cout << "CURRENT DECLINATION: " << std::acos(lookAt.z) << std::endl;
	if (requestedDeclination < zenitMin) {
		look.y = currentDeclination - zenitMin;
	}
	else if (requestedDeclination > zenitMax) {
		look.y = currentDeclination - zenitMax;
	}
	*/

	// ROTATE VECTOR
	glm::vec3 defaultUp = { 0.0, 0.0, 1.0 };
	glm::vec3 right = glm::cross(lookAt, defaultUp);
	glm::vec3 up = glm::cross(right, lookAt);

	glm::vec3 finalLookAt = glm::rotate(glm::mat4(1), look.x, up) * glm::vec4(lookAt, 0);
	finalLookAt = glm::rotate(glm::mat4(1), look.y, right) * glm::vec4(finalLookAt, 0);

	lookAt = glm::lerp(lookAt, finalLookAt, (float)std::fmin(std::fmax(deltaTime * 1000.0f, 0.0), 1.0f));

	right = glm::cross(lookAt, defaultUp);
	up = glm::cross(right, lookAt);

	Input::InputManager::GetInstance().ResetCursorPosition();

	_view = glm::lookAt(position, position + lookAt, up);
}

void Camera::FOVFirstPersonCamera(float deltaTime) {
	/*
		Change fov depending on the mouse rullete
	*/
	double x = 0, y = 0;
	Input::InputManager::GetInstance().GetScrollPosition(x, y);

	//std::cout << "Y SCROLL " << y << std::endl;

	fov -= scrollSpeed * y * deltaTime;

	//std::cout << "FOV: " << fov << std::endl;

	_proj = glm::perspective(glm::radians(fov), aspectRatio, near, far);
	_proj[1][1] *= -1;
}
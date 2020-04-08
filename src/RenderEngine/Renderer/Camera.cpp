#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera() {
	_view = glm::lookAt(glm::vec3(0.0f, -2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	_proj = glm::perspective(glm::radians(45.0f), 16 / 9.0f, 0.1f, 10.0f);

	_proj[1][1] *= -1;
}

Camera::Camera(glm::vec3 eyePosition, glm::vec3 center, glm::vec3 upAxis, float fov, float aspectRatio, float near, float far) {
	_view = glm::lookAt(eyePosition, center, upAxis);
	_proj = glm::perspective(glm::radians(fov), aspectRatio, near, far);


	_proj[1][1] *= -1;
}

glm::mat4 Camera::GetView() const{
	return _view;
}

glm::mat4 Camera::GetProjection() const{
	return _proj;
}

void Camera::SetView(glm::mat4 view) {
	_view = view;
}

void Camera::SetProjection(glm::mat4 proj) {
	_proj = proj;
}
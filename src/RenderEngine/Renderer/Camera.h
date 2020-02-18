#pragma once

#include <glm/glm.hpp>

/**
* Class to represent a camera. For now, it only has two matrix, view and projection, to represent it.
* It has a constructor to generate the view and projection matrix. 
* @author Fernando del Molino
*/
class Camera
{
private:
	glm::mat4 _view;
	glm::mat4 _proj;

public:
	Camera();

	Camera(glm::vec3 eyePosition, glm::vec3 center, glm::vec3 upAxis, float fov, float aspectRatio, float near, float far);

	glm::mat4 GetView() const;

	glm::mat4 GetProjection() const;

};


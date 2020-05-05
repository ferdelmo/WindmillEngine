#pragma once

#include <glm/glm.hpp>

#define _USE_MATH_DEFINES
#include <math.h>


/**
* Class to represent a camera. For now, it only has two matrix, view and projection, to represent it.
* It has a constructor to generate the view and projection matrix. 

* @author Fernando del Molino
*/
class Camera
{
public:
	glm::mat4 _view;
	glm::mat4 _proj;

	glm::vec3 position;

	glm::vec3 lookAt = { 0, 1, 0 };

	float movSpeed = 5.0f;
	float angularSpeed = 1000.0f;
	float scrollSpeed = 1000.0f;

	float aspectRatio, near, far, fov;

	// max zenit in radians
	float zenitMin = M_PI / 180.0f * 5.0f;
	float zenitMax = M_PI - zenitMin;

	double minSensibility = 2.0f;

	void MoveFirstPersonCamera(float deltaTime);

	void OrientFirstPersonCamera(float deltaTime);

	void FOVFirstPersonCamera(float deltaTime);

public:
	Camera();

	Camera(glm::vec3 eyePosition, glm::vec3 center, glm::vec3 upAxis, float fov, float aspectRatio, float near, float far);

	glm::mat4 GetView() const;

	glm::mat4 GetProjection() const;

	void Update(float deltaTime);

};


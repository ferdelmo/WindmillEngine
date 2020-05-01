#include "CollisionShapes.h"

#include <algorithm>

using namespace Physics;


bool Physics::Utils::Collision(const Box* A, const Sphere* B, glm::vec3 position) {
	
	// move the sphere center to the box coordinates

	glm::mat4 matrix = A->GetModel();

	glm::vec3 sphereCenter = glm::inverse(matrix) * glm::vec4(B->GetPosition(), 1);

	//its assumed that the position is in the center for the box

	std::vector<glm::vec3> axis = A->GetNormals();

	glm::vec3 extents = A->GetExtents();

	// Get closest point of the box to the sphere
	
	glm::vec3 closestPoint = { 0,0,0 };

	closestPoint.x = std::max(-extents.x, std::min(extents.x, closestPoint.x));
	closestPoint.y = std::max(-extents.x, std::min(extents.y, closestPoint.y));
	closestPoint.x = std::max(-extents.x, std::min(extents.x, closestPoint.z));

	// take the offset vector

	glm::vec3 dir = sphereCenter - closestPoint;
	float length = glm::length(dir);

	if (length < B->GetRadius()) {
		return true;
	}
	return false;
}
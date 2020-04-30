#include "Sphere.h"


using namespace Physics;


Sphere::Sphere(glm::vec3 pos, float r) : pos(pos), r(r) {

}


bool Sphere::Collision(const Sphere* a, const Sphere* b, glm::vec3& position) {
	// check if the distance between

	glm::vec3 dir = b->pos - a->pos;
	float dist = glm::distance(b->pos, a->pos);

	if (dist < a->r + b->r) {
		// are colliding
		glm::normalize(dir);
		glm::vec3 pointA = a->pos + dir * a->r;
		glm::vec3 pointB = b->pos - dir * b->r;

		position = (pointA + pointB) / 2.0f;
		return true;
	}
	else {
		return false;
	}
}


void Sphere::SetPosition(glm::vec3 pos) {
	this->pos = pos;
}
glm::vec3 Sphere::GetPosition() const {
	return pos;
}

void Sphere::SetRadius(float r) {
	this->r = r;
}
float Sphere::GetRadius() const {
	return r;
}
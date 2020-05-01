#include "Box.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace Physics;

Box::Box(glm::mat4 model, glm::vec3 position, glm::vec3 extents) : model(model), C(position)
	, extents(extents) {

}

/*
	Only checking 6 of the 15 SAT axis needed
*/
// https://www.geometrictools.com/Documentation/DynamicCollisionDetection.pdf
bool Box::Collision(const Box* A, const Box* B, glm::vec3 position) {

	//std::vector<glm::vec3> AVertices = A->GetVertices();
	//std::vector<glm::vec3> BVertices = B->GetVertices();

	std::vector<glm::vec3> ANormals = A->GetNormals();
	std::vector<glm::vec3> BNormals = B->GetNormals();

	glm::vec3 D = B->C - A->C;

	glm::mat3 C;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			C[i][j] = glm::dot(ANormals[i], BNormals[j]);
		}
	}

	// B extents
	float b[3] = {B->extents.x,
		B->extents.y,
		B->extents.z
	};


	// A extents
	float a[3] = { A->extents.x,
		A->extents.y,
		A->extents.z
	};

	// TEST 3 axis A
	for (int i = 0; i < 3; i++) {
		float R0 = a[i];
		float R1 = b[0] * glm::abs(C[i][0]) + b[1] * glm::abs(C[i][1]) + b[2] * glm::abs(C[i][2]);
		float R = glm::abs(glm::dot(ANormals[i], D));

		if (R0 + R1 < R) {
			return false;
		}
	}

	// test 3 axes of B normals
	for (int i = 0; i < 3; i++) {
		float R0 = a[0] * glm::abs(C[0][i]) + a[1] * glm::abs(C[1][i]) + a[2] * glm::abs(C[2][i]);
		float R1 = b[i];
		float R = glm::abs(glm::dot(BNormals[i], D));

		if (R0 + R1 < R) {
			return false;
		}
	}

	// test for A0 vs B0, B1, B2

	return true;

	// test with all the combinations of edges
}


void Box::SetModel(glm::mat4 model) {
	this->model = model;
}
glm::mat4 Box::GetModel() const {
	return model;
}



std::vector<glm::vec3> Box::GetVertices() const {
	std::vector<glm::vec3> vertices = {};

	for (auto& v : boxVertex) {
		vertices.push_back(model * glm::vec4(v, 1));
	}
	
	return vertices;
}

std::vector<glm::vec3> Box::GetNormals() const {
	std::vector<glm::vec3> normals = {};

	for (auto& n : boxNormals) {
		normals.push_back(model * glm::vec4(n, 0));
	}

	return normals;
}

void Box::SetPosition(glm::vec3 pos) {
	C = pos;
}
glm::vec3 Box::GetPosition() const {
	return C;
}

void Box::SetExtents(glm::vec3 extents) {
	this->extents = extents;
}
glm::vec3 Box::GetExtents() const {
	return extents;
}
#pragma once

#include "Shape.h"

#include <vector>

namespace Physics {

	/*
		basic box to rotate with the model matrix
	*/
	const std::vector<glm::vec3> boxVertex = {
		{0.5,-0.5, -0.5}, {-0.5,-0.5, -0.5}, {-0.5,0.5, -0.5}, {-0.5,0.5, -0.5},
		{0.5,-0.5, 0.5}, {-0.5,-0.5, 0.5}, {-0.5,0.5, 0.5}, {-0.5,0.5, 0.5}
	};

	const std::vector<glm::vec3> boxNormals = {
		{1,0,0}, {0,1,0}, {0,0,1}
	};

	class Box : public Shape
	{
	private:
		// without scalling
		glm::mat4 model;

		// extents
		glm::vec3 extents;

		glm::vec3 C;

	public:
		virtual ShapeType GetType() const override {
			return ShapeType::BOX;
		}

		Box(glm::mat4 model, glm::vec3 position, glm::vec3 extents);

		static bool Collision(const Box* A, const Box* B, glm::vec3 position);


		void SetModel(glm::mat4 model);
		glm::mat4 GetModel() const;



		std::vector<glm::vec3> GetVertices() const;
		std::vector<glm::vec3> GetNormals() const;


		void SetPosition(glm::vec3 pos);
		glm::vec3 GetPosition() const;


		void SetExtents(glm::vec3 extents);
		glm::vec3 GetExtents() const;
	};
}


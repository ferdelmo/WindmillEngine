#pragma once
#include "Vertex.h"
#include <vector>
#include <string>

class Mesh
{
public:
	std::vector<VertexNormalMapping> vertices;
	std::vector<Index> indices;

	Mesh();

	static Mesh* LoadMesh(std::string path, float scale);

private:
    static void ComputeTangentBasis(
        // inputs
        std::vector<glm::vec3>& vertices,
        std::vector<glm::vec2>& uvs,
        std::vector<glm::vec3>& normals,
        // outputs
        std::vector<glm::vec3>& tangents,
        std::vector<glm::vec3>& bitangents
    );
};


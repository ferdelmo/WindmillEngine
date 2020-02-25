#pragma once
#include "Vertex.h"
#include <vector>
#include <string>

class Mesh
{
public:
	std::vector<VertexNormal> vertices;
	std::vector<Index> indices;

	Mesh();

	static Mesh* LoadMesh(std::string path, float scale);
};


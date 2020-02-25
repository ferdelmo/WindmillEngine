#pragma once
#include "Vertex.h"
#include <vector>

class Mesh
{
public:
	std::vector<VertexNormal> vertices;
	std::vector<Index> indices;
};


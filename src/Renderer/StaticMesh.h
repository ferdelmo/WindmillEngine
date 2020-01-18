#pragma once
#include "Vertex.h"
#include <vector>
#include "Buffer.h"

/**
* Represent a static mesh with a list of vertex. Initially once initialized, the list of vertex
* should not change (TODO: fix that)
* @author Fernando del Molino
*/
class StaticMesh
{
private:
	std::vector<Vertex> _vertices;
	Buffer _vertexBuffer;

	std::vector<Index> _indices;
	Buffer _indexBuffer;

public:
	/*
		Constructor with a list of vertices and indices
	*/
	StaticMesh(VulkanInstance* vk, const std::vector<Vertex>& vertices, const std::vector<Index>& indices);

	~StaticMesh();

	/*
		Initialize the mesh -> Initialize the needed bufers and rest of vulkan things needed
	*/
	void Initialize();

	/*
		Bind the buffers and the draw orders to a command
	*/
	void BindBufferToCommand(VkCommandBuffer cmd);

	/*
		Get the vertex buffer of the mesh
	*/
	VkBuffer GetVertexBuffer();

	/*
		Get the vertex buffer of the mesh
	*/
	VkBuffer GetIndexBuffer();

};


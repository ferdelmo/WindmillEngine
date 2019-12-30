#pragma once

#include "Utils.h"

#include <vector>

class RenderObject
{
private:
	//VertexBuffer
	Buffer _vertexBuffer;

	//IndexBuffer
	Buffer _indexBuffer;

	//Maybe a vector for eachframe
	//Buffers for the uniforms, and pass info every frame
	Buffer _uniformBuffer;

	//descriptor pool to inform buffers about the uniform
	VkDescriptorPool _descriptorPool;
	//Maybe vector
	VkDescriptorSet _descriptorSets;

	std::vector<Vertex> _vertices;

	typedef uint32_t index;

	std::vector<index> _indices;

public:

	RenderObject(VkDevice device, const std::vector<Vertex>& vertices, const std::vector<index>& indices) :
		_vertexBuffer(device), _indexBuffer(device), _uniformBuffer(device), _vertices(vertices), _indices(indices) {

		_vertexBuffer.Initialize(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory)
	}

};


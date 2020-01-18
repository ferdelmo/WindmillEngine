#include "StaticMesh.h"



StaticMesh::StaticMesh(VulkanInstance* vk, const std::vector<Vertex>& vertices, const std::vector<Index>& indices)
	: _vertices(vertices), _indices(indices), _vertexBuffer(vk), _indexBuffer(vk){
	
}

StaticMesh::~StaticMesh() {

}

/*
	Initialize the mesh -> Initialize the needed bufers and rest of vulkan things needed
*/
void StaticMesh::Initialize() {
	_vertexBuffer.Initialize(_vertices, VK_BUFFER_USAGE_TRANSFER_DST_BIT 
		| VK_BUFFER_USAGE_INDEX_BUFFER_BIT,	VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	_indexBuffer.Initialize(_indices, VK_BUFFER_USAGE_TRANSFER_DST_BIT
		| VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
}

/*
	Bind the buffers and the draw orders to a command
*/
void StaticMesh::BindBufferToCommand(VkCommandBuffer cmd) {
	// TODOO
}

VkBuffer StaticMesh::GetVertexBuffer() {
	return _vertexBuffer.GetBuffer();
}

VkBuffer StaticMesh::GetIndexBuffer() {
	return _indexBuffer.GetBuffer();
}
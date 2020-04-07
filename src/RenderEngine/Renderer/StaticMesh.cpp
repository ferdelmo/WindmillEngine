#include "StaticMesh.h"

#include "../VulkanInstance.h"
#include "Buffer.h"
#include <glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#include "GraphicsPipeline.h"
#include "DescriptorSetLayout.h"
#include "Material.h"

#include <iostream>

StaticMesh::StaticMesh(const std::vector<VertexNormal>& vertices, const std::vector<Index>& indices, Material* mat)
	: _vertices(vertices), _indices(indices), _vertexBuffer(), _indexBuffer(), _material(mat)  {
	_ubo.model = glm::mat4(1.0f);
}

StaticMesh::~StaticMesh() {
	for (auto entry : _uniforms) {
		delete entry.second;
	}

	vkDestroyDescriptorPool(VulkanInstance::GetInstance().device, _descriptorPool, nullptr);
}

/*
	Initialize the mesh -> Initialize the needed bufers and rest of vulkan things needed
*/
void StaticMesh::Initialize() {
	_vertexBuffer.Initialize(_vertices, VK_BUFFER_USAGE_TRANSFER_DST_BIT 
		| VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	_vertexBuffer.isVertex = true;

	_indexBuffer.Initialize(_indices, VK_BUFFER_USAGE_TRANSFER_DST_BIT
		| VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	
	_uniforms = _material->GenerateDescriptorSet(_descriptorPool, _descriptorSet);

}

void StaticMesh::Update(float deltaTime) {
	std::vector<MVP> uniform = { _ubo };
	_uniforms.at("MVP")->Fill(uniform);
}

void StaticMesh::Rotate(float angle, glm::vec3 up) {
	float actualRot = angle;
	_ubo.model = glm::rotate(_ubo.model, glm::radians(actualRot), up);

	std::vector<MVP> uniform = { _ubo };
	_uniforms.at("MVP")->Fill(uniform);
}

void StaticMesh::Translate(glm::vec3 trans) {
	_ubo.model = glm::translate(_ubo.model, trans);
	std::vector<MVP> uniform = { _ubo };
	_uniforms.at("MVP")->Fill(uniform);
}

void StaticMesh::SetCamera(const Camera& cam) {
	_ubo.view = cam.GetView();
	_ubo.proj = cam.GetProjection();
}

VkBuffer& StaticMesh::GetVertexBuffer() {
	return _vertexBuffer.GetBuffer();
}

VkBuffer& StaticMesh::GetIndexBuffer() {
	return _indexBuffer.GetBuffer();
}
/*
VkBuffer& StaticMesh::GetUniformBuffer() {
	return _uniformBuffer.GetBuffer();
}
*/

void StaticMesh::BindCommandsToBuffer(VkCommandBuffer& cmd) {
	VkDeviceSize offsets[] = { 0 };


	vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, _material->GetPipeline().GetPipeline());

	vkCmdBindVertexBuffers(cmd, 0, 1, &_vertexBuffer.GetBuffer(), offsets);

	vkCmdBindIndexBuffer(cmd, _indexBuffer.GetBuffer(), 0, VK_INDEX_TYPE_UINT32);

	vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, _material->GetPipeline().GetPipelineLayout(), 0, 1, &_descriptorSet, 0, nullptr);

	vkCmdDrawIndexed(cmd, static_cast<uint32_t>(_indices.size()), 1, 0, 0, 0);

	//std::cout << "USING VERTEX: " << _vertexBuffer.GetBuffer() << std::endl;
	//std::cout << "USING INDEX: " << _indexBuffer.GetBuffer() << std::endl;
}

std::vector<Index> StaticMesh::GetIndices() {
	return _indices;
}

std::vector<VertexNormal> StaticMesh::GetVertices() {
	return _vertices;
}

MVP& StaticMesh::GetMVP() {
	return _ubo;
}
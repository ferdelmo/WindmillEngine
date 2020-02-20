#include "StaticMesh.h"

#include "../VulkanInstance.h"
#include "Buffer.h"
#include <glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#include "GraphicsPipeline.h"
#include "DescriptorSetLayout.h"
#include "Material.h"

StaticMesh::StaticMesh(const std::vector<Vertex>& vertices, const std::vector<Index>& indices, Material* mat)
	: _vertices(vertices), _indices(indices), _vertexBuffer(), _indexBuffer(), _material(mat)  {
	_ubo.model = glm::mat4(1.0f);
}

StaticMesh::~StaticMesh() {
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
	static float rot = 0;
	float actualRot = 90 * deltaTime;
	rot += actualRot;
	//_ubo.model = glm::rotate(_ubo.model, glm::radians(actualRot), glm::vec3(0.0f, 0.0f, 1.0f));
	/*
	std::vector<MVP> uniform = { _ubo };
	_uniformBuffer.Fill(uniform);
	*/
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

	vkCmdBindIndexBuffer(cmd, _indexBuffer.GetBuffer(), 0, VK_INDEX_TYPE_UINT16);

	vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, _material->GetPipeline().GetPipelineLayout(), 0, 1, &_descriptorSet, 0, nullptr);

	vkCmdDrawIndexed(cmd, static_cast<uint32_t>(_indices.size()), 1, 0, 0, 0);
}

std::vector<Index> StaticMesh::GetIndices() {
	return _indices;
}

std::vector<Vertex> StaticMesh::GetVertices() {
	return _vertices;
}


/*
void StaticMesh::CreateDescriptorPool() {
	std::vector<VkDescriptorPoolSize> poolSizes = { {} };
	poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSizes[0].descriptorCount = static_cast<uint32_t>(1);

	VkDescriptorPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = static_cast<uint32_t>(1);

	if (vkCreateDescriptorPool(VulkanInstance::GetInstance().device, &poolInfo, nullptr, &_descriptorPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor pool!");
	}
}

void StaticMesh::CreateDescriptoSet() {
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = _descriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(1);
	allocInfo.pSetLayouts = &_pipeline->GetDescriptorSetLayout()->GetDescriptor();

	if (vkAllocateDescriptorSets(VulkanInstance::GetInstance().device, &allocInfo, &_descriptorSet) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate descriptor sets!");
	}

	VkDescriptorBufferInfo bufferInfo = {};
	bufferInfo.buffer = _uniformBuffer.GetBuffer();
	bufferInfo.offset = 0;
	bufferInfo.range = sizeof(MVP);

	VkDescriptorImageInfo imageInfo = {};
	imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	imageInfo.imageView = _texture.GetImageView();
	imageInfo.sampler = _texture.GetImageSampler();

	std::vector<VkWriteDescriptorSet> descriptorWrites = { {} };

	descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrites[0].dstSet = _descriptorSet;
	descriptorWrites[0].dstBinding = 0;
	descriptorWrites[0].dstArrayElement = 0;
	descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptorWrites[0].descriptorCount = 1;
	descriptorWrites[0].pBufferInfo = &bufferInfo;
	descriptorWrites[0].pImageInfo = nullptr; // Optional
	descriptorWrites[0].pTexelBufferView = nullptr; // Optional

	vkUpdateDescriptorSets(VulkanInstance::GetInstance().device, static_cast<uint32_t>(descriptorWrites.size()),
		descriptorWrites.data(), 0, nullptr);

}
*/
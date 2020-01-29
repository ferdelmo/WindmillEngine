#include "StaticMesh.h"

#include "../VulkanInstance.h"
#include "Buffer.h"
#include <glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

StaticMesh::StaticMesh(VulkanInstance* vk, const std::vector<Vertex>& vertices, const std::vector<Index>& indices, const std::string texture,
	VkDescriptorSetLayout* descriptorLayout, VkPipelineLayout* pipelineLayout)
	: _vertices(vertices), _indices(indices), _vertexBuffer(vk), _indexBuffer(vk), _uniformBuffer(vk), _pathToTexture(texture), _texture(vk),
	_vk(vk), _descriptorLayout(descriptorLayout), _pipelineLayout(pipelineLayout)  {
	_ubo.model = glm::mat4(1.0f);
}

StaticMesh::~StaticMesh() {
	vkDestroyDescriptorPool(_vk->device, _descriptorPool, nullptr);
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
	
	std::vector<UniformBufferObject> uniform = { _ubo };
	_uniformBuffer.Initialize(uniform, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	_texture.Initialize(_pathToTexture);

	CreateDescriptorPool();
	CreateDescriptoSet();
}

void StaticMesh::Update(float deltaTime) {
	static float rot = 0;
	float actualRot = 90 * deltaTime;
	rot += actualRot;
	//_ubo.model = glm::rotate(_ubo.model, glm::radians(actualRot), glm::vec3(0.0f, 0.0f, 1.0f));
	
	std::vector<UniformBufferObject> uniform = { _ubo };
	_uniformBuffer.Fill(uniform);
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

VkBuffer& StaticMesh::GetUniformBuffer() {
	return _uniformBuffer.GetBuffer();
}

void StaticMesh::BindCommandsToBuffer(VkCommandBuffer& cmd) {
	VkDeviceSize offsets[] = { 0 };

	vkCmdBindVertexBuffers(cmd, 0, 1, &_vertexBuffer.GetBuffer(), offsets);

	vkCmdBindIndexBuffer(cmd, _indexBuffer.GetBuffer(), 0, VK_INDEX_TYPE_UINT16);

	vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, *_pipelineLayout, 0, 1, &_descriptorSet, 0, nullptr);

	vkCmdDrawIndexed(cmd, static_cast<uint32_t>(_indices.size()), 1, 0, 0, 0);
}

std::vector<Index> StaticMesh::GetIndices() {
	return _indices;
}

std::vector<Vertex> StaticMesh::GetVertices() {
	return _vertices;
}



void StaticMesh::CreateDescriptorPool() {
	std::array<VkDescriptorPoolSize, 2> poolSizes = {};
	poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSizes[0].descriptorCount = static_cast<uint32_t>(1);
	poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	poolSizes[1].descriptorCount = static_cast<uint32_t>(1);

	VkDescriptorPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = static_cast<uint32_t>(1);

	if (vkCreateDescriptorPool(_vk->device, &poolInfo, nullptr, &_descriptorPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor pool!");
	}
}

void StaticMesh::CreateDescriptoSet() {
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = _descriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(1);
	allocInfo.pSetLayouts = _descriptorLayout;

	if (vkAllocateDescriptorSets(_vk->device, &allocInfo, &_descriptorSet) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate descriptor sets!");
	}

	VkDescriptorBufferInfo bufferInfo = {};
	bufferInfo.buffer = _uniformBuffer.GetBuffer();
	bufferInfo.offset = 0;
	bufferInfo.range = sizeof(UniformBufferObject);

	VkDescriptorImageInfo imageInfo = {};
	imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	imageInfo.imageView = _texture.GetImageView();
	imageInfo.sampler = _texture.GetImageSampler();

	std::array<VkWriteDescriptorSet, 2> descriptorWrites = {};

	descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrites[0].dstSet = _descriptorSet;
	descriptorWrites[0].dstBinding = 0;
	descriptorWrites[0].dstArrayElement = 0;
	descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptorWrites[0].descriptorCount = 1;
	descriptorWrites[0].pBufferInfo = &bufferInfo;
	descriptorWrites[0].pImageInfo = nullptr; // Optional
	descriptorWrites[0].pTexelBufferView = nullptr; // Optional

	descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrites[1].dstSet = _descriptorSet;
	descriptorWrites[1].dstBinding = 1;
	descriptorWrites[1].dstArrayElement = 0;
	descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	descriptorWrites[1].descriptorCount = 1;
	descriptorWrites[1].pImageInfo = &imageInfo;
	descriptorWrites[1].pTexelBufferView = nullptr; // Optional

	vkUpdateDescriptorSets(_vk->device, static_cast<uint32_t>(descriptorWrites.size()), 
		descriptorWrites.data(), 0, nullptr);

}
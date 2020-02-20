#include "Material.h"
#include "RenderPass.h"
#include "Uniform.h"
#include "RenderEngine/RenderThread/RenderThread.h"
#include <string>

Material::Material() {

}

Material::~Material() {

}

void Material::Initialize(const std::string vert, std::vector<UniformInfo*> vertBinds,
	const std::string frag, std::vector<UniformInfo*> fragBinds,
	RenderPass* renderPass,
	VkVertexInputBindingDescription inputBind,
	std::vector<VkVertexInputAttributeDescription> inputAttr) {

	_vert.Initialize(vert, VK_SHADER_STAGE_VERTEX_BIT, vertBinds);
	_frag.Initialize(frag, VK_SHADER_STAGE_FRAGMENT_BIT, fragBinds);

	RenderThread& rt = RenderThread::GetInstance();

	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)rt.GetExtent().width;
	viewport.height = (float)rt.GetExtent().height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor = {};
	scissor.offset = { 0, 0 };
	scissor.extent = rt.GetExtent();

	_pipeline.Initialize(renderPass, &_vert, &_frag, rt.GetFormat(), viewport, scissor);

	//_texture.Initialize("../resources/textures/texture.jpg");

	for (auto info : vertBinds) {
		AddUniformBuffer(info->name, info);
	}

	for (auto info : fragBinds) {
		AddUniformBuffer(info->name, info);
	}
}

void Material::AddUniformBuffer(std::string name, UniformInfo* value) {
	std::pair<std::string, UniformInfo*> tuple;
	tuple.first = name;
	// Clean buffer to acces more easy to these info
	tuple.second = value;

	if (_uniforms.count(name) > 0) {
		throw std::runtime_error("Material::GenerateDescriptorSet: Trying to add an exisiting name");
	}
	else {
		_uniforms.insert(tuple);
	}
}

std::map<std::string, Buffer*> Material::GenerateDescriptorSet(VkDescriptorPool& descriptorPool, VkDescriptorSet& descriptorSet) {
	/* Create the descrtiptor pool */
	std::vector<VkDescriptorPoolSize> poolSizes = {};
	for (auto entry : _uniforms) {
		VkDescriptorPoolSize aux = {};
		aux.type = entry.second->type;
		aux.descriptorCount = entry.second->descriptorCount;
		poolSizes.push_back(aux);
	}

	VkDescriptorPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = static_cast<uint32_t>(1);

	if (vkCreateDescriptorPool(VulkanInstance::GetInstance().device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor pool!");
	}

	/* Create descriptor set */

	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(1);
	allocInfo.pSetLayouts = &_pipeline.GetDescriptorSetLayout()->GetDescriptor();

	if (vkAllocateDescriptorSets(VulkanInstance::GetInstance().device, &allocInfo, &descriptorSet) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate descriptor sets!");
	}

	std::vector<VkWriteDescriptorSet> descriptorWrites = {};
	std::vector<VkDescriptorBufferInfo*> descriptorWritesBufferInfo = {};

	std::map<std::string, Buffer*> resul;
	for (auto entry : _uniforms) {

		if (entry.second->obj->GetTypeUniform() == UniformTypes::UNIFORM) { // && entry.second->name == "Color") {
			VkWriteDescriptorSet aux = {};

			std::pair<std::string, Buffer*> pair;
			pair.first = entry.second->name;
			pair.second = entry.second->obj->GetUniformBuffer();

			resul.insert(pair);

			VkDescriptorBufferInfo* bufferInfo = new VkDescriptorBufferInfo();
			bufferInfo->buffer = pair.second->GetBuffer();
			bufferInfo->offset = 0;
			bufferInfo->range = entry.second->obj->size;

			descriptorWritesBufferInfo.push_back(bufferInfo);

			aux.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			aux.dstSet = descriptorSet;
			aux.dstBinding = entry.second->binding;
			aux.dstArrayElement = 0;
			aux.descriptorType = entry.second->type;
			aux.descriptorCount = entry.second->descriptorCount;
			aux.pBufferInfo = descriptorWritesBufferInfo.back();
			aux.pImageInfo = nullptr; // Optional
			aux.pTexelBufferView = nullptr; // Optional
			descriptorWrites.push_back(aux);
		}
		else if (entry.second->obj->GetTypeUniform() == UniformTypes::TEXTURE) {
			VkDescriptorImageInfo imageInfo = {};
			imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfo.imageView = *entry.second->obj->GetImageView();
			imageInfo.sampler = *entry.second->obj->GetImageSampler();

			VkWriteDescriptorSet aux = {};
			aux.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			aux.dstSet = descriptorSet;
			aux.dstBinding = entry.second->binding;
			aux.dstArrayElement = 0;
			aux.descriptorType = entry.second->type;
			aux.descriptorCount = entry.second->descriptorCount;
			aux.pImageInfo = &imageInfo;
			aux.pTexelBufferView = nullptr; // Optional
			descriptorWrites.push_back(aux);
		}
	}

	vkUpdateDescriptorSets(VulkanInstance::GetInstance().device, static_cast<uint32_t>(descriptorWrites.size()),
		descriptorWrites.data(), 0, nullptr);

	for (auto aux : descriptorWritesBufferInfo) {
		delete aux;
	}

	return resul;
}
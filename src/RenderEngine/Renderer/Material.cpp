#include "Material.h"
#include "RenderPass.h"
#include "Uniform.h"
#include "SingleThreadRenderer/SingleThreadRenderer.h"
#include "Image.h"
#include <string>
#include <iostream>

Material::Material(std::string materialName) : _materialName(materialName) {

}

Material::~Material() {
	for (auto entry : _uniforms) {
		delete entry.second;
	}
}

void Material::Initialize(const std::string vert, std::vector<UniformInfo*> vertBinds,
	const std::string frag, std::vector<UniformInfo*> fragBinds,
	RenderPass* renderPass,
	VkVertexInputBindingDescription inputBind,
	std::vector<VkVertexInputAttributeDescription> inputAttr) {

	_vert.Initialize(vert, VK_SHADER_STAGE_VERTEX_BIT, vertBinds);
	if (frag != "") {
		_frag.Initialize(frag, VK_SHADER_STAGE_FRAGMENT_BIT, fragBinds);
	}

	SingleThreadRenderer& rt = SingleThreadRenderer::GetInstance();

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

	if (frag != "") {
		_pipeline.Initialize(renderPass, &_vert, &_frag, rt.GetFormat(), inputBind, inputAttr, viewport, scissor);
	}
	else {
		_pipeline.Initialize(renderPass, &_vert, rt.GetFormat(), inputBind, inputAttr, viewport, scissor);
	}

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

MapUniforms Material::GenerateDescriptorSet(VkDescriptorPool& descriptorPool, VkDescriptorSet& descriptorSet) {
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
	std::vector<std::vector<VkDescriptorBufferInfo>> descriptorWritesBufferInfo = {};

	int index = 0;
	MapUniforms resul;
	for (auto entry : _uniforms) {
		if (entry.second->obj->GetTypeUniform() == UniformTypes::UNIFORM) {
			descriptorWritesBufferInfo.push_back(std::vector<VkDescriptorBufferInfo>());
			VkWriteDescriptorSet aux = {};

			std::pair<std::string, MaterialUniform*> pair;
			pair.first = entry.second->name;
			pair.second = new MaterialUniformBuffer(entry.second->obj->GetUniformBuffer());

			resul.insert(pair);

			for (int i = 0; i < entry.second->descriptorCount; i++) {
				VkDescriptorBufferInfo bufferInfo;
				bufferInfo.buffer = pair.second->GetBuffer()->GetBuffer();
				bufferInfo.offset = i* entry.second->obj->size;
				bufferInfo.range = entry.second->obj->size;

				descriptorWritesBufferInfo[index].push_back(bufferInfo);
			}

			aux.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			aux.dstSet = descriptorSet;
			aux.dstBinding = entry.second->binding;
			aux.dstArrayElement = 0;
			aux.descriptorType = entry.second->type;
			aux.descriptorCount = entry.second->descriptorCount;
			aux.pBufferInfo = descriptorWritesBufferInfo[index].data();
			aux.pImageInfo = nullptr; // Optional
			aux.pTexelBufferView = nullptr; // Optional
			descriptorWrites.push_back(aux);

			index++;
		}
		else if (entry.second->obj->GetTypeUniform() == UniformTypes::TEXTURE) {
			if (entry.second->name != "ShadowMap") {
				std::pair<std::string, MaterialUniform*> pair;
				pair.first = entry.second->name;
				Texture* obj = ((UniformTexture*)entry.second->obj)->obj;

				pair.second = new MaterialUniformTexture(obj);

				resul.insert(pair);
			}

			VkDescriptorImageInfo imageInfo = {};
			imageInfo.imageLayout = entry.second->obj->GetImageLayout();
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
	/*
	for (auto aux : descriptorWritesBufferInfo) {
		delete aux;
	}
	*/
	return resul;
}

void Material::UpdateDescriptorSet(VkDescriptorSet& descriptorSet, std::string variable, Texture* tex) {
	std::vector<VkWriteDescriptorSet> descriptorWrites = {};
	VkDescriptorImageInfo imageInfo = {};
	imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	imageInfo.imageView = tex->GetImageView();
	imageInfo.sampler = tex->GetImageSampler();

	VkWriteDescriptorSet aux = {};
	aux.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	aux.dstSet = descriptorSet;
	aux.dstBinding = _uniforms[variable]->binding;
	aux.dstArrayElement = 0;
	aux.descriptorType = _uniforms[variable]->type;
	aux.descriptorCount = _uniforms[variable]->descriptorCount;
	aux.pImageInfo = &imageInfo;
	aux.pTexelBufferView = nullptr; // Optional
	descriptorWrites.push_back(aux);

	vkUpdateDescriptorSets(VulkanInstance::GetInstance().device, static_cast<uint32_t>(descriptorWrites.size()),
		descriptorWrites.data(), 0, nullptr);
}
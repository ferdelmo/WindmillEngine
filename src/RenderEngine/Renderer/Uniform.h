#pragma once
#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

#include "Buffer.h"
#include "Texture.h"

typedef enum class UniformTypes {
	UNIFORM,
	TEXTURE
};

struct UniformInterface {
	virtual Buffer* GetUniformBuffer() { return nullptr; };

	virtual VkImageView* GetImageView() { return nullptr; };

	virtual VkSampler* GetImageSampler() { return nullptr; };

	virtual UniformTypes GetTypeUniform() { return UniformTypes::UNIFORM; };

	VkDeviceSize size;
};

/*
	struct to save different uniform types
*/
template<typename T>
struct UniformType : public UniformInterface {
	T obj;

	UniformType() {
		size = sizeof(T);
	}

	UniformType(T obj) {
		size = sizeof(T);
		this->obj = obj;
	}

	Buffer* GetUniformBuffer() override {
		Buffer* buf = new Buffer();
		buf->Initialize(&obj, size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		return buf;
	}

	UniformTypes GetTypeUniform() override { return UniformTypes::UNIFORM; }

	T GetObject() {
		return obj;
	}
};

struct UniformTexture : public UniformInterface {
	Texture* obj;

	UniformTexture() {
		size = 0;
	}

	UniformTexture(Texture* obj) {
		size = 0;
		this->obj = obj;
	}


	virtual VkImageView *GetImageView() { return &obj->GetImageView(); };

	virtual VkSampler* GetImageSampler() { return &obj->GetImageSampler(); };

	UniformTypes GetTypeUniform() override { return UniformTypes::TEXTURE; }

	Texture* GetTexture() {
		return obj;
	}
};

struct MVP {
	alignas(16) glm::mat4 model;
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;

	MVP(glm::mat4 _model = glm::mat4(1.0f), glm::mat4 _view = glm::mat4(1.0f), glm::mat4 _proj = glm::mat4(1.0f)) :
		model(_model), view(_view), proj(_proj) {
		//size = sizeof(*this);
	}

	/*
	Buffer* GetUniformBuffer() override{
		Buffer* b = new Buffer();
		b->Initialize<MVP>(this, sizeof(*this), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		return b;
	}
	*/
};

struct UniformColor {
	glm::vec3 color;

	UniformColor(glm::vec3 _color = glm::vec3(0,1,0)) : color(_color) {
		//size = sizeof(*this);
	}

	/*
	Buffer* GetUniformBuffer() override{
		Buffer* b = new Buffer();
		b->Initialize<MVP>(this, sizeof(*this), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		return b;
	}
	*/
};

struct UniformInfo {
	// Need a safer method
	UniformInterface* obj;
	std::string name;

	uint16_t binding = 0;
	VkDescriptorType type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uint16_t descriptorCount = 1;
	VkShaderStageFlags stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

	~UniformInfo() {
		delete obj;
	}


	static UniformInfo* GenerateInfoTexture(Texture* obj, std::string name, uint16_t binding,
		uint16_t descriptorCount,
		VkDescriptorType type,
		VkShaderStageFlags stageFlags) {
		UniformInfo* info = new UniformInfo();
		info->obj = new UniformTexture(obj);
		info->name = name;
		info->binding = binding;
		info->type = type;
		info->descriptorCount = descriptorCount;
		info->stageFlags = stageFlags;
		return info;
	}
	template<typename T>
	static UniformInfo* GenerateInfo(T obj, std::string name, uint16_t binding,
		uint16_t descriptorCount,
		VkDescriptorType type,
		VkShaderStageFlags stageFlags) {
		UniformInfo* info = new UniformInfo();
		info->obj = new UniformType<T>(obj);
		info->name = name;
		info->binding = binding;
		info->type = type;
		info->descriptorCount = descriptorCount;
		info->stageFlags = stageFlags;
		return info;
	}
};
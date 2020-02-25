#pragma once
#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

#include "Buffer.h"
#include "Texture.h"

#define MAX_LIGHTS 10

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
	std::vector<T> objs;

	UniformType() {
		size = sizeof(objs[0]);
	}

	UniformType(std::vector<T> objs) {
		size = sizeof(objs[0]);
		this->objs = objs;
	}

	Buffer* GetUniformBuffer() override {
		Buffer* buf = new Buffer();
		buf->Initialize(objs, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		return buf;
	}

	UniformTypes GetTypeUniform() override { return UniformTypes::UNIFORM; }

	std::vector<T> GetObject() {
		return objs;
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

	~UniformTexture() {
		delete obj;
	}

	virtual VkImageView *GetImageView() { return &obj->GetImageView(); };

	virtual VkSampler* GetImageSampler() { return &obj->GetImageSampler(); };

	UniformTypes GetTypeUniform() override { return UniformTypes::TEXTURE; }

	Texture* GetTexture() {
		return obj;
	}
};

/*
	Struct to store the model, view, projection matrix
*/
struct MVP {
	alignas(16) glm::mat4 model;
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;

	MVP(glm::mat4 _model = glm::mat4(1.0f), glm::mat4 _view = glm::mat4(1.0f), glm::mat4 _proj = glm::mat4(1.0f)) :
		model(_model), view(_view), proj(_proj) {
		//size = sizeof(*this);
	}
};

/*
	struct for a uniform color
*/
struct UniformColor {
	alignas(4) glm::vec4 color;

	UniformColor(glm::vec4 _color = glm::vec4(0,1,0,1)) : color(_color) {

	}
};


/*
	Struct to represent a point light
*/
struct PointLight {
	glm::vec3 position;
	float power;
	glm::vec3 color;
	float aux; //used for aligment, but obiusly not the best option

	PointLight(glm::vec3 _pos = glm::vec3(0), float _power = 0, glm::vec3 _color = glm::vec3(1)) :
	position(_pos), power(_power), color(_color) {

	}
};

/* Struct to represent the lights in the scene */
struct Lights {
	PointLight lights[MAX_LIGHTS];
	alignas(4) int num_lights = 0;
};

/* Struct to represent ambient light*/
struct AmbientLight{
	glm::vec3 color = { 0,0,0 };
	float coef = 0.0f;
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
		VkShaderStageFlags stageFlags) {
		UniformInfo* info = new UniformInfo();
		info->obj = new UniformTexture(obj);
		info->name = name;
		info->binding = binding;
		info->type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		info->descriptorCount = 1;
		info->stageFlags = stageFlags;
		return info;
	}

	template<typename T>
	static UniformInfo* GenerateInfo(T obj, std::string name, uint16_t binding,
		VkDescriptorType type,
		VkShaderStageFlags stageFlags) {
		UniformInfo* info = new UniformInfo();
		std::vector<T> aux = { obj };
		info->obj = new UniformType<T>(aux);
		info->name = name;
		info->binding = binding;
		info->type = type;
		info->descriptorCount = 1;
		info->stageFlags = stageFlags;
		return info;
	}

	template<typename T>
	static UniformInfo* GenerateInfoArray(std::vector<T> objs, std::string name, uint16_t binding,
		VkDescriptorType type,
		VkShaderStageFlags stageFlags) {
		UniformInfo* info = new UniformInfo();
		info->obj = new UniformType<T>(objs);
		info->name = name;
		info->binding = binding;
		info->type = type;
		info->descriptorCount = objs.size();
		info->stageFlags = stageFlags;
		return info;
	}
};
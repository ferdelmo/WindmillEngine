#pragma once
#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

#include "Buffer.h"
#include "Texture.h"
#include "Image.h"

#include "Managers/TextureManager.h"

#define MAX_LIGHTS 10

typedef enum class UniformTypes {
	UNIFORM,
	TEXTURE,
	CONSTANT
};


struct UniformInterface {
	virtual ~UniformInterface(){}

	virtual Buffer* GetUniformBuffer() { return nullptr; };

	virtual VkImageView* GetImageView() { return nullptr; };

	virtual VkSampler* GetImageSampler() { return nullptr; };

	virtual VkImageLayout GetImageLayout() { return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL; };

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

	~UniformTexture() {
		//the manager will delete the textures created
		//delete obj;
		TextureManager::GetInstance().UnloadTexture(obj->GetPath());
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

struct UniformImage : public UniformInterface {
	Image* obj;
	VkSampler* sampler;
	VkImageLayout imageLayout;

	~UniformImage() {
		//the manager will delete the textures created
		//delete obj;
	}

	UniformImage() {

	}

	UniformImage(Image* obj, VkSampler* sampler, VkImageLayout imageLayout) {
		size = 0;
		this->obj = obj;
		this->sampler = sampler;
		this->imageLayout = imageLayout;
	}

	virtual VkImageView* GetImageView() { return &obj->GetImageView(); };

	virtual VkSampler* GetImageSampler() { return sampler; };

	UniformTypes GetTypeUniform() override { return UniformTypes::TEXTURE; }

	virtual VkImageLayout GetImageLayout() { return imageLayout; };

	Image* GetImage() {
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
	float power = 0;
	glm::vec3 color;
	float aux1 = 0;
	glm::vec3 LightPosition_cameraspace;
	float aux2 = 0; //used for aligment, but obiusly not the best option

	PointLight(glm::vec3 _pos = glm::vec3(0), float _power = 0, glm::vec3 _color = glm::vec3(1)) :
		position(_pos), power(_power), color(_color), 
		LightPosition_cameraspace({0, 0, 0}) {
		
	}
};

/* Struct to represent a directional light */
struct DirectionalLight {
	alignas(16) glm::mat4 depthBiasMVP;
	glm::vec3 direction;
	float power = 0;
	glm::vec3 color;
	// used for aligment
	float aux1 = 0;

	DirectionalLight() {

	}

	DirectionalLight(glm::vec3 direction, glm::vec3 color, float power) : direction(direction),
		color(color), power(power) {

	}
};

/* Struct to represent the lights in the scene */
struct Lights {
	PointLight lights[MAX_LIGHTS];
	DirectionalLight directionalLights[MAX_LIGHTS];
	alignas(4) int num_lights = 0, num_directional = 0;
};

/* Struct to represent ambient light*/
struct AmbientLight{
	glm::vec3 color = { 0,0,0 };
	float coef = 0.0f;
};


/* struct to represent phong shading info */
struct PhongShading {
	glm::vec3 difusseColor = { 0, 0, 0 };
	float kd;
	glm::vec3 specularColor = { 0, 0, 0 };
	float ks;
	int alpha;
	//aliment
	glm::vec3 aux;

	PhongShading(glm::vec3 kdc, float kd, glm::vec3 kds, float ks, int alpha) : difusseColor(kdc),
	kd(kd), specularColor(kds), ks(ks), alpha(alpha) {

	}
};

struct UniformInfo {
	// Need a safer method
	std::vector<UniformInterface*> obj;
	std::string name;

	/*
		binding and descriptorCount are reused for size and offset when a constant
		is created
	*/

	uint16_t binding = 0;
	VkDescriptorType type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uint16_t descriptorCount = 1;
	VkShaderStageFlags stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

	UniformTypes uniformType = UniformTypes::UNIFORM;

	~UniformInfo() {
		for(auto aux : obj){
			delete aux;
		}
	}


	static UniformInfo* GenerateInfoTexture(Texture* obj, std::string name, uint16_t binding,
		VkShaderStageFlags stageFlags) {
		UniformInfo* info = new UniformInfo();
		info->obj = std::vector<UniformInterface*>(1,new UniformTexture(obj));
		info->name = name;
		info->binding = binding;
		info->type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		info->descriptorCount = 1;
		info->stageFlags = stageFlags;
		info->uniformType = UniformTypes::TEXTURE;
		return info;
	}

	template<typename T>
	static UniformInfo* GenerateInfo(T obj, std::string name, uint16_t binding,
		VkDescriptorType type,
		VkShaderStageFlags stageFlags) {
		UniformInfo* info = new UniformInfo();
		std::vector<T> aux = { obj };
		info->obj = std::vector<UniformInterface*>(1, new UniformType<T>(aux));
		info->name = name;
		info->binding = binding;
		info->type = type;
		info->descriptorCount = 1;
		info->stageFlags = stageFlags;
		info->uniformType = UniformTypes::UNIFORM;
		return info;
	}

	template<typename T>
	static UniformInfo* GenerateInfoArray(std::vector<T> objs, std::string name, uint16_t binding,
		VkDescriptorType type,
		VkShaderStageFlags stageFlags) {
		UniformInfo* info = new UniformInfo();
		info->obj = std::vector<UniformInterface*>(1, new UniformType<T>(objs));
		info->name = name;
		info->binding = binding;
		info->type = type;
		info->descriptorCount = objs.size();
		info->stageFlags = stageFlags;
		info->uniformType = UniformTypes::UNIFORM;
		return info;
	}

	static UniformInfo* GenerateInfoImage(Image* obj, VkSampler* sampler, VkImageLayout imageLayout,
		std::string name, uint16_t binding, VkShaderStageFlags stageFlags) {
		UniformInfo* info = new UniformInfo();
		info->obj = std::vector<UniformInterface*>(1, new UniformImage(obj, sampler, imageLayout));
		info->name = name;
		info->binding = binding;
		info->type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		info->descriptorCount = 1;
		info->stageFlags = stageFlags;
		info->uniformType = UniformTypes::TEXTURE;
		return info;
	}

	static UniformInfo* GenerateInfoImageVector(const std::vector<Image*>& obj, VkSampler* sampler, 
		VkImageLayout imageLayout,
		std::string name, uint16_t binding, VkShaderStageFlags stageFlags) {
		UniformInfo* info = new UniformInfo();
		info->obj = std::vector<UniformInterface*>(obj.size());

		for (int i = 0; i < obj.size(); i++) {
			info->obj[i] = new UniformImage(obj[i], sampler, imageLayout);
		}
		info->name = name;
		info->binding = binding;
		info->type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		info->descriptorCount = obj.size();
		info->stageFlags = stageFlags;
		info->uniformType = UniformTypes::TEXTURE;
		return info;
	}

	static UniformInfo* GenerateConstantInfo(std::string name, uint16_t size, uint16_t offset,
		VkShaderStageFlags stageFlags) {
		UniformInfo* info = new UniformInfo();
		info->obj = std::vector<UniformInterface*>();
		info->name = name;
		info->descriptorCount = 1;
		info->stageFlags = stageFlags;
		info->uniformType = UniformTypes::CONSTANT;

		info->binding = size;
		info->descriptorCount = offset;
		return info;
	}
};
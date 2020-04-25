#pragma once
#include "GraphicsPipeline.h"
#include "Shader.h"
#include "Texture.h"

#include <map>

class UniformInfo;
class RenderPass;

class Material
{
private:
	GraphicsPipeline _pipeline;
	Shader _vert;
	Shader _frag;

	std::map<std::string, UniformInfo*> _uniforms;

	//used as key for the material manager
	std::string _materialName;

public: 
	Material(std::string materialName);

	~Material();

	void Initialize(const std::string vert, std::vector<UniformInfo*> vertBinds,
		const std::string frag, std::vector<UniformInfo*> fragBinds,
		RenderPass* renderPass,
		VkVertexInputBindingDescription inputBind,
		std::vector<VkVertexInputAttributeDescription> inputAttr);

	void AddUniformBuffer(std::string name, UniformInfo* value);

	std::map<std::string, Buffer*> GenerateDescriptorSet(VkDescriptorPool& descriptorPool, VkDescriptorSet& descriptorSet);

	GraphicsPipeline& GetPipeline() {
		return _pipeline;
	}

	std::string GetName() {
		return _materialName;
	}
};


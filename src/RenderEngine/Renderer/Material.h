#pragma once
#include "GraphicsPipeline.h"
#include "Shader.h"
#include "Texture.h"

#include <map>

class UniformInfo;
class RenderPass;
class Buffer;

/*
	abstract class uniform to store uniform buffers and textures
*/
struct MaterialUniform {
	virtual ~MaterialUniform(){}
	virtual Buffer* GetBuffer() { return nullptr; }
	virtual Texture* GetTexture() { return nullptr; }
	virtual void SetTexture(Texture* tex) { }
};

struct MaterialUniformBuffer : public MaterialUniform {
	Buffer* buf;

	MaterialUniformBuffer(Buffer* buf) : buf(buf) {

	}

	~MaterialUniformBuffer(){
		delete buf;
	}

	virtual Buffer* GetBuffer() override { return buf; }
};

struct MaterialUniformTexture : public MaterialUniform {
	Texture* tex;

	MaterialUniformTexture(Texture* tex) : tex(tex) {

	}

	~MaterialUniformTexture() {
		//textures are deleted from the texture manager
		//delete tex;
		TextureManager::GetInstance().UnloadTexture(this->tex->GetPath());
	}

	virtual Texture* GetTexture() override { return tex; }
	virtual void SetTexture(Texture* tex) {
		/*
		if (this->tex != nullptr) {
			TextureManager::GetInstance().UnloadTexture(this->tex->GetPath());
		}
		*/
		this->tex = tex; }
};


typedef std::map<std::string, MaterialUniform*> MapUniforms;

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

	MapUniforms GenerateDescriptorSet(VkDescriptorPool& descriptorPool, VkDescriptorSet& descriptorSet);

	GraphicsPipeline& GetPipeline() {
		return _pipeline;
	}

	std::string GetName() {
		return _materialName;
	}
	
	void UpdateDescriptorSet(VkDescriptorSet& descriptorSet, std::string variable, Texture* tex);
};


#pragma once

#include <map>
#include <vector>
#include <string>

#include "Buffer.h"
#include "GraphicsPipeline.h"

class Material;

/*
	Class to represent a material instance, 
	to reuse same pipeline a shader with different parameters
*/
class MaterialInstance
{
private:
	Material* _parent;

	std::string _materialName;

	std::map<std::string, Buffer*> _uniforms;

	VkDescriptorPool _descriptorPool = VK_NULL_HANDLE;
	VkDescriptorSet _descriptorSet = VK_NULL_HANDLE;

	bool _init = false;

public:

	/*
		The material is supposed to be passed with the materialUtils functions
	*/
	MaterialInstance(std::string materialName);

	~MaterialInstance();

	/*
		clean the things of the last parent and change the material parent
	*/
	void LoadMaterial(std::string materialName);

	// to acces pipeline, shaders and other stuff
	Material* GetMaterial();
	
	/*
		Funtion to change material instance atributes,

		THIS IS VERY UNSECURE, NEED TO CHECK TYPES AND BUFFER SIZES
	*/
	template<typename T>
	bool SetValue(std::string variable, T value) {
		if (_uniforms.find(variable) != _uniforms.end()) {
			// the key exists

			std::vector<T> uni = { value };
			_uniforms.at(variable)->Fill(uni);

			return true;
		}
		else {
			return false;
		}
	}

	VkDescriptorSet& GetDescriptorSet();

	GraphicsPipeline& GetPipeline();
};


#include "MaterialInstance.h"

#include "MaterialManager.h"
#include "Material.h"

/*
	The material is supposed to be passed with the materialUtils functions
*/
MaterialInstance::MaterialInstance(std::string materialName) :
	_materialName(materialName) {

	_parent = MaterialManager::GetInstance().GetMaterial(materialName);

	if (_parent != nullptr) {
		_init = true;
		_uniforms = _parent->GenerateDescriptorSet(_descriptorPool, _descriptorSet);
	}
}

MaterialInstance::~MaterialInstance() {
	if (!_init) {
		// only clean stuff if the material was initialized
		return;
	}


	for (auto entry : _uniforms) {
		delete entry.second;
	}
	vkDestroyDescriptorPool(VulkanInstance::GetInstance().device, _descriptorPool, nullptr);
}

/*
		clean the things of the last parent and change the material parent
	*/
void MaterialInstance::LoadMaterial(std::string materialName) {
	if (_init) {
		for (auto entry : _uniforms) {
			delete entry.second;
		}
		vkDestroyDescriptorPool(VulkanInstance::GetInstance().device, _descriptorPool, nullptr);
	}

	_init = false;
	_parent = MaterialManager::GetInstance().GetMaterial(materialName);

	if (_parent != nullptr) {
		_init = true;
		_uniforms = _parent->GenerateDescriptorSet(_descriptorPool, _descriptorSet);
	}
}

// to acces pipeline, shaders and other stuff
Material* MaterialInstance::GetMaterial() {
	return _parent;
}

VkDescriptorSet& MaterialInstance::GetDescriptorSet() {
	return _descriptorSet;
}

GraphicsPipeline& MaterialInstance::GetPipeline() {
	return _parent->GetPipeline();
}
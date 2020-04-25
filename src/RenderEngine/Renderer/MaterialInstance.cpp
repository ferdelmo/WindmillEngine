#include "MaterialInstance.h"

#include "MaterialManager.h"
#include "Material.h"

/*
	The material is supposed to be passed with the materialUtils functions
*/
MaterialInstance::MaterialInstance(std::string materialName) :
	_materialName(materialName) {

	_parent = MaterialManager::GetInstance().GetMaterial(materialName);
}

MaterialInstance::~MaterialInstance() {
	for (auto entry : _uniforms) {
		delete entry.second;
	}
}

// to acces pipeline, shaders and other stuff
Material* MaterialInstance::GetMaterial() {
	return _parent;
}


std::map<std::string, ParameterInfo*>& MaterialInstance::GetValues() {
	return _uniforms;
}
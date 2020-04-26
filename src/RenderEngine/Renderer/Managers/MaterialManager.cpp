#include "MaterialManager.h"
#include "Material.h"

#include <iostream>

MaterialManager* MaterialManager::_instance = nullptr;

MaterialManager::MaterialManager() {

}

MaterialManager::~MaterialManager() {
	for (auto entry : _materials) {
		delete entry.second.material;
	}
}

MaterialManager& MaterialManager::GetInstance() {
	if (_instance == nullptr) {
		_instance = new MaterialManager();
	}
	return *_instance;
}

void MaterialManager::CleanUp() {
	delete _instance;
}

// return a material instance
Material* MaterialManager::GetMaterial(std::string name) {
	Material* resul = nullptr;

	if (_materials.find(name) != _materials.end()) {
		resul = _materials.at(name).material;
	}

	return resul;
}

void MaterialManager::AddMaterial(std::string name, Material* material) {
	if (_materials.find(name) == _materials.end()) {
		_materials[name] = { material };
	}
	else {
		std::cout << "Material with name \"" << name << "\" alredy exists." << std::endl;
	}
}
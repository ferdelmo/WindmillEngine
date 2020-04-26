#include "TextureManager.h"

TextureManager* TextureManager::_instance = nullptr;

TextureManager::TextureManager() {

}

TextureManager::~TextureManager() {
	for (auto& entry : _textures) {
		//delete entry.second.texture;
	}
}

TextureManager& TextureManager::GetInstance() {
	if (_instance == nullptr) {
		_instance = new TextureManager();
	}

	return *_instance;
}


void TextureManager::CleanUp() {
	delete _instance;
}

// return a material instance
Texture* TextureManager::LoadTexture(std::string path) {
	if (_textures.find(path) == _textures.end()) {
		_textures[path].texture.Initialize(path);
	}

	//_textures[path].references++;
	
	return &_textures[path].texture;
}

// return a material instance
void TextureManager::UnloadTexture(std::string path) {
	if (_textures.find(path) != _textures.end()) {
		_textures[path].references--;
	}

	//std::cout << _textures[path].references << std::endl;
}
#include "MeshManager.h"

#include <iostream>

MeshManager* MeshManager::_instance = nullptr;

MeshManager::MeshManager() {

}

MeshManager::~MeshManager() {
	for (auto& entry : _buffers) {
		delete entry.second.mesh;
		delete entry.second.vertex;
		delete entry.second.index;
	}
}

MeshManager& MeshManager::GetInstance() {
	if (_instance == nullptr) {
		_instance = new MeshManager();
	}

	return *_instance;
}


void MeshManager::CleanUp() {
	delete _instance;
}



MeshManager::MeshInfo* MeshManager::LoadMesh(std::string path) {
	if (_buffers.find(path) == _buffers.end()) {
		_buffers[path].path = path;
		_buffers[path].mesh = Mesh::LoadMesh(path, 1);

		Buffer* vertex = new Buffer();
		vertex->Initialize(_buffers[path].mesh->vertices, VK_BUFFER_USAGE_TRANSFER_DST_BIT
			| VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		Buffer* index = new Buffer();
		index->Initialize(_buffers[path].mesh->indices, VK_BUFFER_USAGE_TRANSFER_DST_BIT
			| VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		_buffers[path].vertex = vertex;
		_buffers[path].index = index;
	}

	_buffers[path].references++;

	std::cout << _buffers[path].references << std::endl;

	return &_buffers[path];
}

void MeshManager::UnloadMesh(std::string path) {
	if (_buffers.find(path) != _buffers.end()) {
		_buffers[path].references--;
	}

	std::cout << _buffers[path].references << std::endl;
	// maybe erase when no references left
}
#include "World.h"

#include "StaticMeshComponent.h"
#include <iostream>

World::World() : _objects(0) {
	_activeWorld = this;
}
World::~World() {

}

World* World::_activeWorld = nullptr;

void World::AddObject(GameObject* go) {
	if (go != nullptr) {
		go->SetWorld(this);
		_objects.push_back(go);
	}
}

void World::RemoveObject(GameObject* go) {
	for (auto it = _objects.begin(); it != _objects.end(); ++it) {
		if (*it == go) {
			_objects.erase(it);
			it = _objects.end();
			break;
		}
	}
}

std::vector<GameObject*>& World::GetObjects() {
	return _objects;
}


void World::SetLights(const SceneLight& l) {
	_lights = l;
}

SceneLight& World::GetLights() {
	return _lights;
}

void World::SetCamera(const Camera& cam) {
	_camera = cam;
}

Camera& World::GetCamera() {
	return _camera;
}

void World::Initialize() {
	for (auto& go : _objects) {
		go->Initialize();
	}
}

void World::Start() {
	for (auto& go : _objects) {
		go->Start();
	}
}

void World::Update(float deltaTime) {
	int i = 0;
	while(i<_objects.size()) {
		//std::cout << "OBJECT: " << (*iter) << std::endl;
		_objects[i]->Update(deltaTime);
		i++;
	}
}

void World::End() {
	for (auto& go : _objects) {
		go->End();
	}
}

void World::Clean() {
	for (auto& go : _objects) {
		delete go;
	}

	_objects.empty();
}


World& World::GetActiveWorld() {
	return *_activeWorld;
}

void World::AddPendingToRemove(GameObject* go) {
	if (go != nullptr) {
		_pendingToRemove.push_back(go);
	}
}

void World::DeletePendingToRemove() {
	GameObject* aux = nullptr;
	for (int i = 0; i < _pendingToRemove.size(); ++i) {
		aux = _pendingToRemove.at(i);
		RemoveObject(aux);
		_pendingToRemove.erase(_pendingToRemove.begin() + i);
		delete aux;
		--i;
	}

	_pendingToRemove.empty();
}
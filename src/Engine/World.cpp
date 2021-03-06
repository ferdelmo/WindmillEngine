#include "World.h"

#include "StaticMeshComponent.h"


World* World::_world = nullptr;

World::World() : _objects(0){
	_world = this;
}
World::~World() {

}

World* World::GetActiveWorld() {
	return _world;
}

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
	for (auto& go : _objects) {
		go->Update(deltaTime);
	}
}

void World::End() {
	for (auto& go : _objects) {
		go->End();
	}
}